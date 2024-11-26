#include <pcosynchro/pcosemaphore.h>

class BridgeManagerFloat {
public:
    BridgeManagerFloat(float _maxWeight)
        : bridgeWaiting(0), mutex(1), maxWeight(static_cast<int>(_maxWeight)), currentWeight(0) {}

    ~BridgeManagerFloat() {}

    void access(float weight) {
        while (true) {
            mutex.acquire();
            if (currentWeight + weight <= maxWeight) {
                currentWeight += weight;
                mutex.release();
                break;
            } else {
                mutex.release();
                bridgeWaiting.acquire();
            }
        }
    }

    void leave(float weight) {
        mutex.acquire();
        currentWeight -= weight;
        mutex.release();

        bridgeWaiting.release();
    }

protected:
    PcoSemaphore bridgeWaiting;
    PcoSemaphore mutex;
    int maxWeight;
    int currentWeight;
};
