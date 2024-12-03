#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcoconditionvariable.h>

class BridgeManagerFloat {
public:
    BridgeManagerFloat(float _maxWeight) : availableWeight(_maxWeight) {}

    ~BridgeManagerFloat() {}

    void access(float weight) {
        mutex.lock();

        while(weight > availableWeight) {
            condition.wait(&mutex);
        }
        availableWeight -= weight;
        condition.notifyOne();
        mutex.unlock();
    }

    void leave(float weight) {
        mutex.lock();
        availableWeight += weight;
        condition.notifyOne();
        mutex.unlock();
    }

protected:
    PcoMutex mutex;
    PcoConditionVariable condition;
    int availableWeight;
};
