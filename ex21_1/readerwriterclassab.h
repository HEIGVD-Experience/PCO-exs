/** @file readerwriterclassab.h
 *  @brief Reader-writer with equal priority between two classes
 *
 *  Implementation of a reader-writer resource manager with equal
 *  priority for both classes. Mutual exclusion between classes must be
 *  ensured
 *
 *
 *
 *  @author Yann Thoma
 *  @date 15.05.2017
 *  @bug No known bug
 */

#ifndef READERWRITERCLASSAB_H
#define READERWRITERCLASSAB_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcohoaremonitor.h>


class ReaderWriterClassAB
{
protected:
    PcoSemaphore mutex, semA, semB;
    int nbWaiting = 0, nbIn = 0;
    bool isA = false;

public:
    ReaderWriterClassAB() : mutex(1), semA(0), semB(0)
    {}

    void lockA() {
        mutex.acquire();
        while(nbIn > 0 && !isA) {
            nbWaiting++;
            mutex.release();
            semA.acquire();
            mutex.acquire();
        }
        nbIn++;
        isA = true;
        mutex.release();
    }

    void unlockA() {
        mutex.acquire();
        nbIn--;
        if(nbIn == 0 && nbWaiting > 0) {
            for(int i = 0 ; i < nbWaiting ; i++) {
                semB.release();
            }
            nbWaiting = 0;
        }
        mutex.release();
    }

    void lockB() {
        mutex.acquire();
        while(nbIn > 0 && isA) {
            nbWaiting++;
            mutex.release();
            semB.acquire();
            mutex.acquire();
        }
        nbIn++;
        isA = false;
        mutex.release();
    }

    void unlockB() {
        mutex.acquire();
        nbIn--;
        if(nbIn == 0 && nbWaiting > 0) {
            for(int i = 0 ; i < nbWaiting ; i++) {
                semA.release();
            }
            nbWaiting = 0;
        }
        mutex.release();
    }
};
#endif // READERWRITERCLASSAB_H
