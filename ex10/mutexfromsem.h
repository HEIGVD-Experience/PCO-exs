#include <pcosynchro/pcosemaphore.h>
#include <iostream>


class MutexFromSem
{
protected:

public:
    MutexFromSem()
    {

    }

    ~MutexFromSem()
    {

    }

    void lock()
    {
    }

    void unlock()
    {
    }

    //! Returns true if the mutex was successfully acquired, otherwise returns false without waiting if it is already locked
    bool trylock()
    {
        return false;
    }
};
