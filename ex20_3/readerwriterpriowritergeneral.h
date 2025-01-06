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
      public AbstractReaderWriter, PcoHoareMonitor {
protected:
	Condition waitRead, waitWrite;
	int nbReaders, nbWriters, nbWritersWaiting, nbReadersWaiting;

public:
  ReaderWriterPrioWriterGeneral() :
    nbReaders(0),
	nbWriters(0),
    nbWritersWaiting(0),
    nbReadersWaiting(0)
    {}

    void lockReading() override {
		monitorIn();
        nbReadersWaiting++;
		if(nbWriters > 0 || nbWritersWaiting > 0) {
			wait(waitRead);
		}
        nbReadersWaiting--;
		nbReaders++;
		monitorOut();
    }

    void unlockReading() override {
  		monitorIn();
    	nbReaders--;
		if(nbReaders == 0) {
			signal(waitWrite);
		}
  		monitorOut();
    }

	void lockWriting() override {
		monitorIn();
		nbWritersWaiting++;
		if(nbReaders > 0 || nbWriters > 0) {
			wait(waitWrite);
		}
        nbWritersWaiting--;
        nbWriters++;
		monitorOut();
	}

	void unlockWriting() override {
  		monitorIn();
        nbWriters--;
	    if(nbWritersWaiting > 0) {
        	signal(waitWrite);
        } else {
        	signal(waitRead);
        }
  		monitorOut();
	}
};
#endif // READERWRITERPRIOWRITERGENERAL_H
