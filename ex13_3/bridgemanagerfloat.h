#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcohoaremoniteor.h>

class BridgeManagerFloat {
public:
    BridgeManagerFloat(float _maxWeight) : availableWeight(_maxWeight) {}

    ~BridgeManagerFloat() {}

    void access(float weight) {
        monitorIn();

        while (availableWeight < weight) {
            wait(freeWeight);
        }

        availableWeight -= weight;
        signal(freeWeight);

        monitorOut();
    }

    void leave(float weight) {
        monitorIn();
        availableWeight += weight;
        signal(freeWeight);
        monitorOut();
    }

protected:
    int availableWeight;
    Condition freeWeight;
};
