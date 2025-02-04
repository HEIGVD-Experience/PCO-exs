#include <iostream>

#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include <pcosynchro/pcohoaremonitor.h>

class SemaphoreFromMesaMonitor
{
private:
    PcoConditionVariable cond;
    PcoMutex mutex;
    unsigned int value;

public:
    SemaphoreFromMesaMonitor(unsigned int initValue) : value(initValue)
    {

    }

    ~SemaphoreFromMesaMonitor()
    {

    }

    void wait()
    {
        mutex.lock();
        while (value == 0)
        {
            cond.wait(&mutex);
        }
        value--;
        mutex.unlock();
    }

    void post()
    {
        mutex.lock();
        value++;
        cond.notifyOne();
        mutex.unlock();
    }

    bool trywait()
    {
        mutex.lock();
        if(value > 0)
        {
            value--;
            mutex.unlock();
            return true;
        }
        mutex.unlock();
        return false;
    }  //! Returns true if the semaphore could be and has been acquired, false if it is already blocked
};



class SemaphoreFromHoareMonitor : public PcoHoareMonitor
{
private:

    Condition cond;
    unsigned int value;

public:
    SemaphoreFromHoareMonitor(unsigned int initValue) : value(initValue)
    {

    }

    ~SemaphoreFromHoareMonitor()
    {

    }

    // Attention ici, PcoHoareMonitor a une méthode wait(condition).
    // Pour l'appeler, utilisez PcoHoareMonitor::wait(condition);

    void wait()
    {
        monitorIn();
        if (value == 0)
            PcoHoareMonitor::wait(cond);

        value--;
        monitorOut();
    }

    void post()
    {
        monitorIn();
        value++;
        PcoHoareMonitor::signal(cond);
        monitorOut();
    }

    bool trywait()
    {
        monitorIn();
        if (value > 0)
        {
            value--;
            monitorOut();
            return true;
        }
        monitorOut();
        return false;
    }  //! Returns true if the semaphore could be and has been acquired, false if it is already blocked
};
