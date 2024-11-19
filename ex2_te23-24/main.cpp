
#include <iostream>
#include <algorithm>

#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcosemaphore.h>

class RacletteTic {
  protected:
    PcoSemaphore* waitingE, waitingP, mutex;
    int nbWaiting = 0, nbWaitingPer = 0;
  public:
    RacletteTic() : waitingE(0), waitingP(0), mutex(0);
    void etudiant(){
      mutex->acquire();
      nbWaiting++;
      mutex->release();

      waitingE->acquire();
    }
    void per(){
      mutex->acquire();
      nbWaitingPer++;
      mutex->release();

      waitingP->acquire();
    }
    int racleur(){
      int sortie;

      mutex->acquire();

      if(nbWaitingPer > 0 && nbWaiting < 4) {
        sortie = 1 + nbWaiting;
        nbWaiting--;


      racleur_sem->acquire();

      return 0;
    }
};

int main(int argc, char **argv) {

  return EXIT_SUCCESS;
}
