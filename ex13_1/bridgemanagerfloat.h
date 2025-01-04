#include <pcosynchro/pcosemaphore.h>

class BridgeManagerFloat {
public:
    BridgeManagerFloat(float maxWeight)
        : maxWeight(maxWeight), currentWeight(0), bridgeAvailable(0), mutex(1) {}

    ~BridgeManagerFloat() {}

    void access(float weight) {
        mutex.acquire();
        while (currentWeight + weight > maxWeight) {
            mutex.release();
            bridgeAvailable.acquire();
            mutex.acquire();
        }
        currentWeight += weight;
        mutex.release();
    }

    void leave(float weight) {
        mutex.acquire();
        currentWeight -= weight;
        bridgeAvailable.release();
        mutex.release();
    }

protected:
    float maxWeight, currentWeight;
    PcoSemaphore bridgeAvailable, mutex;
};
