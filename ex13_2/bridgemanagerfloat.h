#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcoconditionvariable.h>

class BridgeManagerFloat {
public:
    BridgeManagerFloat(float maxWeight) : maxWeight(maxWeight), currentWeight(0) {}

    ~BridgeManagerFloat() {}

    void access(float weight) {
        mutex.lock();
        while(currentWeight + weight > maxWeight) {
            condition.wait(&mutex);
        }
        currentWeight += weight;
        mutex.unlock();
    }

    void leave(float weight) {
        mutex.lock();
        currentWeight -= weight;
        condition.notifyAll();
        mutex.unlock();
    }

protected:
    PcoMutex mutex;
    PcoConditionVariable condition;
    float maxWeight, currentWeight;
};
