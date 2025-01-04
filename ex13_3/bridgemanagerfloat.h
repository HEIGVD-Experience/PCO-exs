#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcohoaremoniteor.h>

class BridgeManagerFloat {
public:
    BridgeManagerFloat(float maxWeight) : maxWeight(maxWeight), currentWeight(0) {}

    ~BridgeManagerFloat() {}

    void access(float weight) {
        monitorIn();
        if (currentWeight + weight > maxWeight) {
            wait(isFree);
        }

        currentWeight += weight;
        monitorOut();
    }

    void leave(float weight) {
        monitorIn();
        currentWeight -= weight;
        signal(isFree);
        monitorOut();
    }

protected:
    float maxWeight, currentWeight;
    Condition isFree;
};
