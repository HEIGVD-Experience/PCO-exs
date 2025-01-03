
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

const unsigned int CARWEIGHT = 1;
const unsigned int TRUCKWEIGHT = 10;

class BridgeManager
{
protected:
    unsigned int maxWeight;
    unsigned int currentWeight;
    unsigned int nbWaiting;
    PcoMutex mutex;
    PcoConditionVariable isFull;
public:
    BridgeManager(unsigned int maxWeight) : maxWeight(maxWeight), currentWeight(0), nbWaiting(0)
    {

    }

    ~BridgeManager()
    {

    }

    void carAccess()
    {
        mutex.lock();
        nbWaiting++;
        while (currentWeight + CARWEIGHT > maxWeight)
        {
            mutex.unlock();
            isFull.wait(&mutex);
        }
        nbWaiting--;
        currentWeight += CARWEIGHT;
        mutex.unlock();
    }

    void truckAccess()
    {
        mutex.lock();
        nbWaiting++;
        while (currentWeight + TRUCKWEIGHT > maxWeight)
        {
            mutex.unlock();
            isFull.wait(&mutex);
        }
        nbWaiting--;
        currentWeight += TRUCKWEIGHT;
        mutex.unlock();
    }

    void carLeave()
    {
        mutex.lock();
        currentWeight -= CARWEIGHT;
        isFull.notifyOne();
        mutex.unlock();
    }

    void truckLeave()
    {
        mutex.lock();
        currentWeight -= TRUCKWEIGHT;
        isFull.notifyAll();
        mutex.unlock();
    }
};
