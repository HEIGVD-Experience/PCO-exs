
#include <pcosynchro/pcohoaremonitor.h>


const unsigned int CARWEIGHT = 1;
const unsigned int TRUCKWEIGHT = 10;

class BridgeManager : public PcoHoareMonitor
{
private:
    unsigned int maxWeight;
    unsigned int currentWeight;
    Condition isFree;
public:
    BridgeManager(unsigned int maxWeight) : maxWeight(maxWeight), currentWeight(0)
    {

    }

    ~BridgeManager()
    {

    }

    void carAccess()
    {
        monitorIn();
        while (currentWeight + CARWEIGHT > maxWeight)
        {
            wait(isFree);
        }
        currentWeight += CARWEIGHT;
        monitorOut();
    }

    void truckAccess()
    {
        monitorIn();
        while (currentWeight + TRUCKWEIGHT > maxWeight)
        {
            wait(isFree);
        }
        currentWeight += TRUCKWEIGHT;
        monitorOut();
    }

    void carLeave()
    {
        monitorIn();
        currentWeight -= CARWEIGHT;
        signal(isFree);
        monitorOut();
    }

    void truckLeave()
    {
        monitorIn();
        currentWeight -= TRUCKWEIGHT;
        signal(isFree);
        monitorOut();
    }
};
