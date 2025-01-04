#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include <pcosynchro/pcosemaphore.h>

#include "abstractbuffer.h"

template<typename T> class Buffer2ConsoSemaphore : public AbstractBuffer<T> {
protected:
    PcoSemaphore mutex, waitEmpty, waitFull;
    int nbConso;
    const int MAX_CONSO = 2;
    T element;

public:
    Buffer2ConsoSemaphore() : mutex(1), waitEmpty(1), waitFull(0), nbConso(0) {}

    virtual ~Buffer2ConsoSemaphore() {}

    virtual void put(T item) {
        waitEmpty.acquire();
        mutex.acquire();
        element = item;
        nbConso = 0;
        mutex.release();
        waitFull.release();
        waitFull.release();
    }
    virtual T get() {
        waitFull.acquire();
        mutex.acquire();
        if (nbConso == MAX_CONSO) {
            waitEmpty.release();
            mutex.release();
            return {};
        }
        nbConso++;
        mutex.release();
        return element;
    }
};


#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

template<typename T> class Buffer2ConsoMesa : public AbstractBuffer<T> {
protected:

public:
    Buffer2ConsoMesa() {}

    virtual ~Buffer2ConsoMesa() {}

    virtual void put(T item) {}
    virtual T get() { return {};}
};


#include <pcosynchro/pcohoaremonitor.h>

template<typename T> class Buffer2ConsoHoare : public AbstractBuffer<T>, public PcoHoareMonitor {
protected:
    Condition isEmpty, isFull;
    const int MAX_CONSO = 2;
    int nbConso = MAX_CONSO;
    T element;

public:
    Buffer2ConsoHoare() {}

    virtual ~Buffer2ConsoHoare() {}

    virtual void put(T item) {
        monitorIn();
        if(nbConso < MAX_CONSO) {
            wait(isEmpty);
        }
        element = item;
        nbConso = 0;
        for(int i = 0 ; i < MAX_CONSO ; ++i)
            signal(isFull);
        monitorOut();
    }
    virtual T get() {
        monitorIn();
        if(nbConso == MAX_CONSO)
            wait(isFull);
        nbConso++;
        if(nbConso == MAX_CONSO)
          signal(isEmpty);
        monitorOut();
        return element;
    }
};


#endif // BUFFER2CONSO_H
