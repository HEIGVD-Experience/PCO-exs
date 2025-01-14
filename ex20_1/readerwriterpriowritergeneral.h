/** @file readerwritepriowritergeneral.h
 *  @brief Reader-writer with priority for readers
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for readers. This class follows the "general" way of solving
 *  concurrent problems: internal variables, one mutex, and semaphores
 *  as synchronization primitives.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */

#ifndef READERWRITERPRIOWRITERGENERAL_H
#define READERWRITERPRIOWRITERGENERAL_H

#include <pcosynchro/pcosemaphore.h>
#include <pcosynchro/pcohoaremonitor.h>

#include "abstractreaderwriter.h"


class ReaderWriterPrioWriterGeneral :
      public AbstractReaderWriter {
protected:
  PcoSemaphore mutexNbReaders, mutexNbWriters;

  PcoSemaphore lockRead;
  int nbReaders;

  PcoSemaphore lockWrite;
  int nbWriters;

  PcoSemaphore lockReaders;

public:
  ReaderWriterPrioWriterGeneral() :
    mutexNbReaders(1),
    mutexNbWriters(1),
    lockRead(1),
    lockWrite(1),
    lockReaders(1),
    nbReaders(0),
    nbWriters(0)
    {}

    void lockReading() override {
        lockReaders.acquire();
        lockRead.acquire();
        mutexNbReaders.acquire();
        nbReaders++;
        if(nbReaders == 1) {
            lockWrite.acquire();
        }
        mutexNbReaders.release();
        lockRead.release();
        lockReaders.release();
    }

    void unlockReading() override {
        mutexNbReaders.acquire();
        nbReaders--;
        if(nbReaders == 0) {
            lockWrite.release();
        }
        mutexNbReaders.release();
    }

    void lockWriting() override {
        mutexNbWriters.acquire();
        nbWriters++;
        if(nbWriters == 1) {
            lockRead.acquire();
        }
        mutexNbWriters.release();
        lockWrite.acquire();
    }

    void unlockWriting() override {
        lockWrite.release();
        mutexNbWriters.acquire();
        nbWriters--;
        if(nbWriters == 0) {
            lockRead.release();
        }
        mutexNbWriters.release();
    }
};
#endif // READERWRITERPRIOWRITERGENERAL_H
