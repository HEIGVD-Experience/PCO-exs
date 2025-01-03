
#include <pcosynchro/pcosemaphore.h>

const unsigned int CARWEIGHT = 1;
const unsigned int TRUCKWEIGHT = 10;

class BridgeManager
{
protected:
    unsigned int maxWeight, currentWeight, nbWaiting;

    PcoSemaphore access, mutex;

public:
    BridgeManager(unsigned int maxWeight) : maxWeight(maxWeight), currentWeight(0), access(0), mutex(1), nbWaiting(0)
    {

    }

    ~BridgeManager()
    {

    }

    void carAccess()
    {
        mutex.acquire();
        nbWaiting++;
        while (currentWeight + CARWEIGHT > maxWeight)
        {
            mutex.release();
            access.acquire();
            mutex.acquire();
        }
        currentWeight += CARWEIGHT;
        nbWaiting--;
        mutex.release();
    }

    void truckAccess()
    {
        mutex.acquire();
        nbWaiting++;
        while (currentWeight + TRUCKWEIGHT > maxWeight)
        {
            mutex.release();
            access.acquire();
            mutex.acquire();
        }
        currentWeight += TRUCKWEIGHT;
        nbWaiting--;
        mutex.release();
    }

    void carLeave()
    {
        mutex.acquire();
        currentWeight -= CARWEIGHT;
        for(unsigned int i = 0; i < nbWaiting; i++)
        {
            access.release();
        }
        mutex.release();
    }

    void truckLeave()
    {
        mutex.acquire();
        currentWeight -= TRUCKWEIGHT;
        for(unsigned int i = 0; i < nbWaiting; i++)
        {
            access.release();
        }
        mutex.release();
    }
};
