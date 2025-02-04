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
public:
    ToiletAMesa(int nbSeats) : AbstractToilet(nbSeats)
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

class ToiletAHoare : public AbstractToilet, public PcoHoareMonitor
{
public:
    ToiletAHoare(int nbSeats) : AbstractToilet(nbSeats)
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

class ToiletBSemaphore : public AbstractToilet
{
private:
 	PcoSemaphore mutex, lockMan, lockWoman;
    int nbMan = 0, nbWoman = 0, nbWomanWaiting = 0;
public:
    ToiletBSemaphore(int nbSeats) : AbstractToilet(nbSeats), lockMan(0), lockWoman(0), mutex(1)
    {}

    void manAccessing() override {
        mutex.acquire();
        mutex.release();
    }

    void manLeaving() override {
		mutex.acquire();
      	mutex.release();
    }

    void womanAccessing() override {
        mutex.acquire();
        mutex.release();
    }

    void womanLeaving() override {
        mutex.acquire();
        mutex.release();
    }
};

class ToiletBMesa : public AbstractToilet
{
public:
    ToiletBMesa(int nbSeats) : AbstractToilet(nbSeats)
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

class ToiletBHoare : public AbstractToilet, public PcoHoareMonitor
{
public:
    ToiletBHoare(int nbSeats) : AbstractToilet(nbSeats)
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

class ToiletCSemaphore : public AbstractToilet
{
public:
    ToiletCSemaphore(int nbSeats) : AbstractToilet(nbSeats)
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

class ToiletCMesa : public AbstractToilet
{
public:
    ToiletCMesa(int nbSeats) : AbstractToilet(nbSeats)
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

class ToiletCHoare : public AbstractToilet, public PcoHoareMonitor
{
public:
    ToiletCHoare(int nbSeats) : AbstractToilet(nbSeats)
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
