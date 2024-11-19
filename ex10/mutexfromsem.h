#include <pcosynchro/pcosemaphore.h>
#include <iostream>


class MutexFromSem
{
protected:
PcoSemaphore* mutex = new PcoSemaphore(1);
PcoSemaphore* locked_protector = new PcoSemaphore(1);
bool locked = false;

public:
    MutexFromSem()
    {

    }

    ~MutexFromSem()
    {

    }

    void lock()
    {
      locked_protector->acquire();

      if(locked) {
        locked_protector->release();
        return;
      }

      mutex->acquire();
      locked = true;

      locked_protector->release();
    }

    void unlock()
    {
      locked_protector->acquire();

      if(!locked) {
        locked_protector->release();
        return;
      }

      locked = false;
      mutex->release();
      locked_protector->release();
    }

    //! Returns true if the mutex was successfully acquired, otherwise returns false without waiting if it is already locked
    bool trylock()
    {

      locked_protector->acquire();

      if(!locked) {
        locked_protector->release();
        this->lock();
        return true;
      }

      locked_protector->release();
      return false;

    }
};
