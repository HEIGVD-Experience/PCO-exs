
#include <pcosynchro/pcosemaphore.h>


class BridgeManagerFloat
{
public:
    BridgeManagerFloat(float _maxWeight)
    {
        this->maxWeight = (int)_maxWeight - 1;
        for(int i = 0 ; i < maxWeight ; ++i)
        {
            bridgeControl.release();
        }

    }

    ~BridgeManagerFloat()
    {

    }

    void access(float weight)
    {
        mutex->acquire();
        if(currentWeight + weight < maxWeight)
        {
            for(int i = 0 ; i < weight; ++i)
                bridgeControl.acquire();
        } else
        {
            bridgeControl.acquire();
        }

    }

    void leave(float weight)
    {

    }
protected:
    PcoSemaphore bridgeControl;
    PcoSemaphore* mutex = new PcoSemaphore(0);
    int maxWeight, currentWeight = 0;
};
