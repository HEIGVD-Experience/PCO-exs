/** @file toilets.h
 *  @brief Reader-writer for toilets sharing
 *
 *  Implementation of a reader-writer resource manager for toilets sharing.
 *  Mutual exclusion between classes must be ensured
 *
 *
 *
 *  @author Yann Thoma
 *  @date 25.01.2022
 *  @bug No known bug except it is not implemented
 */

#ifndef TOILETS_H
#define TOILETS_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include <pcosynchro/pcohoaremonitor.h>

#include "abstracttoilets.h"


class ToiletASemaphore : public AbstractToilet
{
private:
    PcoSemaphore mutex;

    PcoSemaphore semWomanWait;
    size_t nbWomanWait;

    PcoSemaphore semManWait;
    size_t nbManWait;

    size_t nbIn;
    bool isWomanIn;
public:
    ToiletASemaphore(int nbSeats) : AbstractToilet(nbSeats), mutex(1), semManWait(0), nbManWait(0), semWomanWait(0), nbWomanWait(0)
    {}

    void manAccessing() override {
        mutex.acquire();

        // Tant qu'il y a des femmes dans les toilettes ou que le nombre de places est atteint on attend
        while((nbIn != 0 && isWomanIn) || nbIn >= nbSeats) {
            ++nbManWait;
            mutex.release();
            semManWait.acquire();
            mutex.acquire();
        }

        ++nbIn;
        isWomanIn = false;
        mutex.release();
    }

    void manLeaving() override {
        mutex.acquire();
        --nbIn;

        if(nbManWait > 0) {
            --nbManWait;
            semManWait.release();
        } else if(nbWomanWait > 0 && nbIn == 0) {
            for(int i = 0; i < nbWomanWait; ++i) semWomanWait.release();
            nbWomanWait = 0;
        }

        mutex.release();
    }

    void womanAccessing() override {
        mutex.acquire();

        while((nbIn != 0 && !isWomanIn) || nbIn >= nbSeats) {
            ++nbWomanWait;
            mutex.release();
            semWomanWait.acquire();
            mutex.acquire();
        }

        ++nbIn;
        isWomanIn = true;
        mutex.release();
    }

    void womanLeaving() override {
        mutex.acquire();
        --nbIn;

        if(nbWomanWait > 0) {
            --nbWomanWait;
            semWomanWait.release();
        } else if(nbManWait > 0 && nbIn == 0) {
            for(int i = 0; i < nbManWait; ++i) semManWait.release();
            nbManWait = 0;
        }

        mutex.release();
    }
};


class ToiletAMesa : public AbstractToilet
{
private:
    PcoMutex mutex;
    PcoConditionVariable condMan, condWoman;

    size_t nbIn, nbManWait, nbWomanWait;
    bool isWomanIn;

public:
    ToiletAMesa(int nbSeats) : AbstractToilet(nbSeats), nbIn(0), nbManWait(0), nbWomanWait(0), isWomanIn(false)
    {}

    void manAccessing() override {
        mutex.lock();

        while((nbIn != 0 && isWomanIn) || nbIn >= nbSeats) {
            ++nbManWait;
            condMan.wait(&mutex);
        }

        ++nbIn;
        isWomanIn = false;
        mutex.unlock();
    }

    void manLeaving() override {
        mutex.lock();
        --nbIn;

        if(nbManWait > 0) {
            --nbManWait;
            condMan.notifyOne();
        } else if(nbWomanWait > 0 && nbIn == 0) {
            condWoman.notifyAll();
            nbWomanWait = 0;
        }

        mutex.unlock();
    }

    void womanAccessing() override {
        mutex.lock();

        while((nbIn != 0 && !isWomanIn) || nbIn >= nbSeats) {
            ++nbWomanWait;
            condWoman.wait(&mutex);
        }

        ++nbIn;
        isWomanIn = true;
        mutex.unlock();
    }

    void womanLeaving() override {
        mutex.lock();
        --nbIn;

        if(nbWomanWait > 0) {
            --nbWomanWait;
            condWoman.notifyOne();
        } else if(nbManWait > 0 && nbIn == 0) {
            condMan.notifyAll();
            nbManWait = 0;
        }
        mutex.unlock();
    }
};

class ToiletAHoare : public AbstractToilet, public PcoHoareMonitor
{
private:
    int nbIn, nbManWait, nbWomanWait;
    bool isWomanIn;
    Condition manCond, womanCond;
public:
    ToiletAHoare(int nbSeats) : AbstractToilet(nbSeats), nbIn(0), nbManWait(0), nbWomanWait(0), isWomanIn(false)
    {}

    void manAccessing() override {
        monitorIn();
        if ((nbIn != 0 && isWomanIn) || nbIn >= nbSeats) {
            ++nbManWait;
            wait(manCond);
            --nbManWait;
        }
        ++nbIn;
        isWomanIn = false;
        monitorOut();
    }

    void manLeaving() override {
        monitorIn();
        --nbIn;
        if (nbManWait > 0) {
            signal(manCond);
        } else if (nbWomanWait > 0 && nbIn == 0) {
            for(size_t i = 0 ; i < nbWomanWait ; i++) signal(womanCond);
        }
        monitorOut();
    }

    void womanAccessing() override {
        monitorIn();
        if ((nbIn != 0 && !isWomanIn) || nbIn >= nbSeats) {
            ++nbWomanWait;
            wait(womanCond);
            --nbWomanWait;
        }
        ++nbIn;
        isWomanIn = true;
        monitorOut();
    }

    void womanLeaving() override {
        monitorIn();
        --nbIn;
        if (nbWomanWait > 0) {
            signal(womanCond);
        } else if (nbManWait > 0 && nbIn == 0) {
            for(size_t i = 0 ; i < nbManWait ; i++) signal(manCond);
        }
        monitorOut();
    }
};

class ToiletBSemaphore : public AbstractToilet
{
private:
    PcoSemaphore mutex;

    PcoSemaphore semWomanWait;
    int nbWomanWait;

    PcoSemaphore semManWait;
    int nbManWait;

    size_t nbIn;
    bool isWomanIn;
public:
    ToiletBSemaphore(int nbSeats) : AbstractToilet(nbSeats), mutex(1), semManWait(0), nbManWait(0),
                                  semWomanWait(0), nbWomanWait(0), nbIn(0), isWomanIn(false)
    {}

    void manAccessing() override {
        mutex.acquire();
        // Si des femmes sont en attente, on mettera les hommes en attente
        while((nbIn > 0 && isWomanIn) || nbWomanWait > 0 || nbIn >= nbSeats) {
            ++nbManWait;
            mutex.release();
            semManWait.acquire();
            mutex.acquire();
        }
        ++nbIn;
        isWomanIn = false;
        mutex.release();
    }

    void manLeaving() override {
		mutex.acquire();
        --nbIn;
        // Les hommes peuvent venir s'il n'y a pas de femmes en attente
        if(nbManWait > 0 && nbWomanWait == 0) {
            --nbManWait;
            semManWait.release();
        } else if(nbWomanWait > 0 && nbIn == 0) {
            for(int i = 0; i < nbWomanWait; ++i) semWomanWait.release();
            nbWomanWait = 0;
        }
      	mutex.release();
    }

    void womanAccessing() override {
        mutex.acquire();
        while((nbIn > 0 && !isWomanIn) || nbIn >= nbSeats) {
            ++nbWomanWait;
            mutex.release();
            semWomanWait.acquire();
            mutex.acquire();
        }
        ++nbIn;
        isWomanIn = true;
        mutex.release();
    }

    void womanLeaving() override {
        mutex.acquire();
        --nbIn;
        if(nbWomanWait > 0) {
            --nbWomanWait;
            semWomanWait.release();
        } else if(nbManWait > 0 && nbIn == 0) {
            for(int i = 0; i < nbManWait; ++i) semManWait.release();
            nbManWait = 0;
        }
        mutex.release();
    }
};

class ToiletBMesa : public AbstractToilet
{
private:
    PcoMutex mutex;
    PcoConditionVariable condMan, condWoman;

    size_t nbIn, nbManWait, nbWomanWait;
    bool isWomanIn;
public:
    ToiletBMesa(int nbSeats) : AbstractToilet(nbSeats), nbIn(0), nbManWait(0), nbWomanWait(0), isWomanIn(false)
    {}

    void manAccessing() override {
        mutex.lock();
        while((nbIn > 0 && isWomanIn) || nbWomanWait > 0 || nbIn >= nbSeats) {
            ++nbManWait;
            condMan.wait(&mutex);
        }
        ++nbIn;
        isWomanIn = false;
        mutex.unlock();
    }

    void manLeaving() override {
        mutex.lock();
        --nbIn;
        if(nbManWait > 0 && nbWomanWait == 0) {
            --nbManWait;
            condMan.notifyOne();
        } else if(nbWomanWait > 0 && nbIn == 0) {
            condWoman.notifyAll();
            nbWomanWait = 0;
        }
        mutex.unlock();
    }

    void womanAccessing() override {
        mutex.lock();
        while((nbIn > 0 && !isWomanIn) || nbIn >= nbSeats) {
            ++nbWomanWait;
            condWoman.wait(&mutex);
        }
        ++nbIn;
        isWomanIn = true;
        mutex.unlock();
    }

    void womanLeaving() override {
        mutex.lock();
        --nbIn;
        if(nbWomanWait > 0) {
            --nbWomanWait;
            condWoman.notifyOne();
        } else if(nbManWait > 0 && nbIn == 0) {
            condMan.notifyAll();
            nbManWait = 0;
        }
        mutex.unlock();
    }
};

class ToiletBHoare : public AbstractToilet, public PcoHoareMonitor
{
private:

    Condition waitWoman, waitMan;

    int nbIn, nbManWait, nbWomanWait, toRelease;
    bool isWomanIn;

public:
    ToiletBHoare(int nbSeats) : AbstractToilet(nbSeats), nbIn(0), nbManWait(0), nbWomanWait(0),
                              isWomanIn(false), toRelease(0)
    {}

    void manAccessing() override {
        monitorIn();
        if ((nbIn > 0 && isWomanIn) || nbWomanWait > 0 || nbIn >= nbSeats) {
            ++nbManWait;
            wait(waitMan);
            --nbManWait;
        }
        ++nbIn;
        monitorOut();
    }

    void manLeaving() override {
        monitorIn();
        --nbIn;
        if (nbManWait > 0 && nbWomanWait == 0) {
            signal(waitMan);
        } else if (nbWomanWait > 0 && nbIn == 0) {
            for(size_t i = 0 ; i < nbWomanWait ; i++) signal(waitWoman);
        }
        monitorOut();
    }

    void womanAccessing() override {
        monitorIn();
        if ((nbIn > 0 && !isWomanIn) || nbIn >= nbSeats) {
            ++nbWomanWait;
            wait(waitWoman);
            --nbWomanWait;
        }
        ++nbIn;
        isWomanIn = true;
        monitorOut();
    }

    void womanLeaving() override {
        monitorIn();
        --nbIn;
        if (nbWomanWait > 0) {
            signal(waitWoman);
        } else if (nbManWait > 0 && nbIn == 0) {
			isWomanIn = false;
            for(size_t i = 0 ; i < nbManWait ; i++) signal(waitMan);
        }
        monitorOut();
    }
};

#include <iostream>
#include <queue>

// Pour résoudre cet exercice j'ai du ajouter 2 secondes pour que l'éxécution ait le temps de terminer
class ToiletCSemaphore : public AbstractToilet {
private:
    PcoSemaphore mutex, semManWait, semWomanWait;
    std::queue<char> toEnter;
    int nbIn;
    bool isWomanIn;

public:
    ToiletCSemaphore(int nbSeats) : AbstractToilet(nbSeats), mutex(1), semManWait(0),
                                    semWomanWait(0), nbIn(0), isWomanIn(false) {}

    void manAccessing() override {
        mutex.acquire();
        while ((nbIn > 0 && isWomanIn) || nbIn >= nbSeats || (!toEnter.empty() && isWomanIn)) {
            toEnter.push('M');
            mutex.release();
            semManWait.acquire();
            mutex.acquire();
        }
        ++nbIn;
        isWomanIn = false;
        mutex.release();
    }

    void manLeaving() override {
        mutex.acquire();
        --nbIn;
        if (nbIn > 0 && !toEnter.empty() && toEnter.front() == 'M') {
            toEnter.pop();
            semManWait.release();
        } else if (nbIn == 0) {
            isWomanIn = false;
            while (!toEnter.empty() && toEnter.front() == 'F') {
                toEnter.pop();
                semWomanWait.release();
            }
        }
        mutex.release();
    }

    void womanAccessing() override {
        mutex.acquire();
        while ((nbIn > 0 && !isWomanIn) || nbIn >= nbSeats || (!toEnter.empty() && !isWomanIn)) {
            toEnter.push('F');
            mutex.release();
            semWomanWait.acquire();
            mutex.acquire();
        }
        ++nbIn;
        isWomanIn = true;
        mutex.release();
    }

    void womanLeaving() override {
        mutex.acquire();
        --nbIn;
        if (nbIn > 0 && !toEnter.empty() && toEnter.front() == 'F') {
            toEnter.pop();
            semWomanWait.release();
        } else if (nbIn == 0) {
            isWomanIn = false;
            while (!toEnter.empty() && toEnter.front() == 'M') {
                toEnter.pop();
                semManWait.release();
            }
        }
        mutex.release();
    }
};

class ToiletCMesa : public AbstractToilet
{
private:
    PcoMutex mutex;
    PcoConditionVariable manWait, womanWait;
    std::queue<char> toEnter;
    int nbIn;
    bool isWomanIn;

public:
    ToiletCMesa(int nbSeats) : AbstractToilet(nbSeats), nbIn(0), isWomanIn(false)
    {}

    void manAccessing() override {
        mutex.lock();
        while((nbIn > 0 && isWomanIn) || nbIn >= nbSeats || (!toEnter.empty() && isWomanIn)) {
            toEnter.push('M');
            manWait.wait(&mutex);
        }
        ++nbIn;
        isWomanIn = false;
        mutex.unlock();
    }

    void manLeaving() override {
        mutex.lock();
        --nbIn;
        if(nbIn > 0 && !toEnter.empty() && toEnter.front() == 'M') {
            toEnter.pop();
            manWait.notifyOne();
        } else if(nbIn == 0) {
            isWomanIn = false;
            while(!toEnter.empty() && toEnter.front() == 'F') {
                toEnter.pop();
                womanWait.notifyOne();
            }
        }
        mutex.unlock();
    }

    void womanAccessing() override {
        mutex.lock();
        while((nbIn > 0 && !isWomanIn) || nbIn >= nbSeats || (!toEnter.empty() && !isWomanIn)) {
            toEnter.push('F');
            womanWait.wait(&mutex);
        }
        ++nbIn;
        isWomanIn = true;
        mutex.unlock();
    }

    void womanLeaving() override {
        mutex.lock();
        --nbIn;
        if(nbIn > 0 && !toEnter.empty() && toEnter.front() == 'F') {
            toEnter.pop();
            womanWait.notifyOne();
        } else if(nbIn == 0) {
            isWomanIn = false;
            while(!toEnter.empty() && toEnter.front() == 'M') {
                toEnter.pop();
                manWait.notifyOne();
            }
        }
        mutex.unlock();
    }
};

class ToiletCHoare : public AbstractToilet, public PcoHoareMonitor
{
private:
    Condition manWait, womanWait;
    std::queue<char> toEnter;
    int nbIn;
    bool isWomanIn;

public:
    ToiletCHoare(int nbSeats) : AbstractToilet(nbSeats), nbIn(0), isWomanIn(false)
    {}

    void manAccessing() override {
        monitorIn();
        if ((nbIn > 0 && isWomanIn) || nbIn >= nbSeats || (!toEnter.empty() && isWomanIn)) {
            toEnter.push('M');
            wait(manWait);  // Attend jusqu'à être signalé
        }
        ++nbIn;
        isWomanIn = false;
        monitorOut();
    }

    void manLeaving() override {
        monitorIn();
        --nbIn;
        if (nbIn > 0 && !toEnter.empty() && toEnter.front() == 'M') {
            toEnter.pop();
            signal(manWait);  // Signale un autre homme si possible
        } else if (nbIn == 0) {
            isWomanIn = true;
            while (!toEnter.empty() && toEnter.front() == 'F') {
                toEnter.pop();
                signal(womanWait);  // Signale une femme si possible
            }
        }
        monitorOut();
    }

    void womanAccessing() override {
        monitorIn();
        if ((nbIn > 0 && !isWomanIn) || nbIn >= nbSeats || (!toEnter.empty() && !isWomanIn)) {
            toEnter.push('F');
            wait(womanWait);  // Attend jusqu'à être signalé
        }
        ++nbIn;
        isWomanIn = true;
        monitorOut();
    }

    void womanLeaving() override {
        monitorIn();
        --nbIn;
        if (nbIn > 0 && !toEnter.empty() && toEnter.front() == 'F') {
            toEnter.pop();
            signal(womanWait);  // Signale une autre femme si possible
        } else if (nbIn == 0) {
            isWomanIn = false;
            while (!toEnter.empty() && toEnter.front() == 'M') {
                toEnter.pop();
                signal(manWait);  // Signale un homme si possible
            }
        }
        monitorOut();
    }
};


class ToiletDSemaphore : public AbstractToilet
{
    int N;

public:
    ToiletDSemaphore(int nbSeats, int N = 3) : AbstractToilet(nbSeats), N(N)
    {}

    void manAccessing() override {
    }

    void manLeaving() override {
    }

    void womanAccessing() override {
    }

    void womanLeaving() override {
    }
};

class ToiletDMesa : public AbstractToilet
{
    int N;

public:
    ToiletDMesa(int nbSeats, int N = 3) : AbstractToilet(nbSeats), N(N)
    {}

    void manAccessing() override {
    }

    void manLeaving() override {
    }

    void womanAccessing() override {
    }

    void womanLeaving() override {
    }
};

class ToiletDHoare : public AbstractToilet, public PcoHoareMonitor
{
    int N;

public:
    ToiletDHoare(int nbSeats, int N = 3) : AbstractToilet(nbSeats), N(N)
    {}

    void manAccessing() override {
    }

    void manLeaving() override {
    }

    void womanAccessing() override {
    }

    void womanLeaving() override {
    }
};

#endif // TOILETS_H
