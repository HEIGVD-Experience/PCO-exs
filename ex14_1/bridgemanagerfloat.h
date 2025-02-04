
#include <pcosynchro/pcosemaphore.h>

#include "vehicle.h"

class BridgeManagerFloat
{
private:
    float maxWeight;
    float currentWeight;
    int waiting;
    PcoSemaphore bridge, mutex;

public:
    BridgeManagerFloat(float maxWeight) : maxWeight(maxWeight), currentWeight(0), bridge(0), mutex(1), waiting(0)
    {

    }

    ~BridgeManagerFloat()
    {

    }

    void access(Vehicle *vehicle)
    {
        mutex.acquire();
        waiting++;
        while (currentWeight + vehicle->getWeight() > maxWeight)
        {
            vehicle->stop();
            mutex.release();
            bridge.acquire();
            mutex.acquire();
            vehicle->start();
        }
        currentWeight += vehicle->getWeight();
        waiting--;
        mutex.release();
    }

    void leave(Vehicle *vehicle)
    {
        mutex.acquire();
        currentWeight -= vehicle->getWeight();
        for(int i = 0; i < waiting; i++)
        {
            bridge.release();
        }
        mutex.release();
    }
};
