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

#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

#include "abstractreaderwriter.h"


class ReaderWriterPrioWriterGeneral :
      public AbstractReaderWriter {
protected:
  PcoMutex mutex;

  PcoConditionVariable waitRead;
  int nbReaders;

  PcoConditionVariable waitWrite;
  int nbWriters, nbWritersWaiting;

public:
  ReaderWriterPrioWriterGeneral() :
    nbReaders(0),
    nbWriters(0)
    {}

    void lockReading() override {
			mutex.lock();
      while (nbWriters > 0 || nbWritersWaiting > 0) {
				waitRead.wait(&mutex);
			}
			nbReaders++;
			mutex.unlock();
    }

    void unlockReading() override {
			mutex.lock();
      nbReaders--;
      if (nbReaders == 0) {
        waitWrite.notifyOne();
			}
      mutex.unlock();
    }

    void lockWriting() override {
			mutex.lock();
			nbWritersWaiting++;
      while (nbReaders > 0 || nbWriters > 0) {
        waitWrite.wait(&mutex);
			}
      nbWritersWaiting--;
      nbWriters++;
      mutex.unlock();

    }

    void unlockWriting() override {
			mutex.lock();
			nbWriters--;
			if (nbWritersWaiting > 0) {
				waitWrite.notifyOne();
			} else {
				waitRead.notifyAll();
			}
			mutex.unlock();
    }
};
#endif // READERWRITERPRIOWRITERGENERAL_H
