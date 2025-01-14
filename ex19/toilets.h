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


class ToiletASemaphore : public AbstractToilet {
private:
    PcoSemaphore mutex, lockHomme, lockFemme;
    int nbFemme = 0, nbHomme = 0, nbFemmeWaiting = 0;

public:
    ToiletASemaphore(int nbSeats)
        : AbstractToilet(nbSeats), mutex(1), lockHomme(1), lockFemme(1) {}

    void manAccessing() override {
        mutex.acquire();
        while (nbFemme > 0 || nbFemmeWaiting > 0 || nbHomme == nbSeats) {
            mutex.release();
            lockHomme.acquire();
            mutex.acquire();
        }
        nbHomme++;
        mutex.release();
    }

    void manLeaving() override {
        mutex.acquire();
        nbHomme--;
        if (nbFemmeWaiting > 0) {
            lockFemme.release();
        } else if (nbHomme < nbSeats) {
            lockHomme.release();
        }
        mutex.release();
    }

    void womanAccessing() override {
        mutex.acquire();
        nbFemmeWaiting++;
        while (nbHomme > 0 || nbFemme == nbSeats) {
            mutex.release();
            lockFemme.acquire();
            mutex.acquire();
        }
        nbFemmeWaiting--;
        nbFemme++;
        if (nbFemme == 1) {
            lockHomme.acquire(); // Bloquer l'accès des hommes
        }
        mutex.release();
    }

    void womanLeaving() override {
        mutex.acquire();
        nbFemme--;
        if (nbFemme > 0) {
            lockFemme.release(); // Permettre à d'autres femmes d'entrer
        } else if (nbFemmeWaiting == 0) {
            lockHomme.release(); // Permettre aux hommes d'entrer
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
public:
    ToiletBSemaphore(int nbSeats) : AbstractToilet(nbSeats)
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
