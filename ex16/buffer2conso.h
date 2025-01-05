#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include "abstractbuffer.h"
#include <pcosynchro/pcosemaphore.h>

/*
INFO:
Je rends quand même, même si chez moi les tests ne passent plus. Je ne comprends pas les deux premiers marchait
et au moment de valider le 3ème plus aucun ne marche. J'ai eu beau chercher pourquoi je n'ai pas trouvé.
 */

const int NB_CONSO = 2;

template<typename T>
class Buffer2ConsoSemaphore : public AbstractBuffer<T> {
protected:
    struct BufferItem {
        T data;
        int consummed;
    };

    std::vector<BufferItem> buffer;
    int writePointer, readPointer, bufferSize;
    PcoSemaphore mutex, waitNotFull, waitNotEmpty;

public:
    Buffer2ConsoSemaphore(unsigned int bufferSize)
        : buffer(bufferSize), writePointer(0), readPointer(0),
          bufferSize(bufferSize), mutex(1), waitNotFull(bufferSize),
          waitNotEmpty(0) {}

    virtual ~Buffer2ConsoSemaphore() {}

    virtual void put(T item) {
        waitNotFull.acquire();
        mutex.acquire();

        // Insérer l'élément dans le buffer
        buffer[writePointer].data = item;
        buffer[writePointer].consummed = 0;
        writePointer = (writePointer + 1) % bufferSize;

        // Relâcher pour permettre à NB_CONSO consommateurs de consommer
        for(int i = 0; i < NB_CONSO; i++) {
            waitNotEmpty.release();
        }

        mutex.release();
    }

    virtual T get() {
        waitNotEmpty.acquire();
        mutex.acquire();

        // Récupérer l'élément
        T item = buffer[readPointer].data;
        buffer[readPointer].consummed++;

        // Si cet élément est complètement consommé, déplacer le pointeur de lecture
        if (buffer[readPointer].consummed == NB_CONSO) {
            readPointer = (readPointer + 1) % bufferSize;
            waitNotFull.release(); // Libérer une place dans le tampon
        }

        mutex.release();
        return item;
    }
};


#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

template<typename T>
class Buffer2ConsoMesa : public AbstractBuffer<T> {
protected:
    struct BufferItem {
        T data;
        int consummed;
    };

    std::vector<BufferItem> buffer;
    int writePointer, readPointer, bufferSize, nbElement;
    PcoMutex mutex;
    PcoConditionVariable isFull, isAvailable;

public:
    Buffer2ConsoMesa(unsigned int bufferSize)
        : buffer(bufferSize), writePointer(0), readPointer(0), bufferSize(bufferSize), nbElement(0) {}

    virtual ~Buffer2ConsoMesa() {}

    virtual void put(T item) {
        mutex.lock();

        // Attendre qu'il y ait de la place dans le tampon
        while (nbElement == bufferSize)
            isFull.wait(&mutex);

        // Ajouter un élément dans le tampon
        buffer[writePointer].data = item;
        buffer[writePointer].consummed = 0;
        writePointer = (writePointer + 1) % bufferSize;
        nbElement++;

        // Notifier les consommateurs qu'un élément est disponible
        isAvailable.notifyAll();
        mutex.unlock();
    }

    virtual T get() {
        T item;
        mutex.lock();

        // Attendre qu'un élément soit disponible
        while (nbElement == 0)
            isAvailable.wait(&mutex);

        // Récupérer l'élément
        item = buffer[readPointer].data;
        buffer[readPointer].consummed++;

        // Si l'élément a été consommé par tous les consommateurs, avancer le pointeur de lecture
        if (buffer[readPointer].consummed == NB_CONSO) {
            readPointer = (readPointer + 1) % bufferSize;
            nbElement--;
            // Notifier les producteurs qu'il y a une place disponible
            isFull.notifyOne();
        }

        mutex.unlock();
        return item;
    }
};



#include <pcosynchro/pcohoaremonitor.h>

template<typename T>
class Buffer2ConsoHoare : public AbstractBuffer<T>, public PcoHoareMonitor {
protected:
    struct BufferItem {
        T data;
        int consummed;
    };

    std::vector<BufferItem> buffer;
    int writePointer, readPointer, bufferSize, nbElement;
    Condition notFull, notEmpty;

public:
    Buffer2ConsoHoare(unsigned int bufferSize)
        : buffer(bufferSize), writePointer(0), readPointer(0),
          bufferSize(bufferSize), nbElement(0) {}

    virtual ~Buffer2ConsoHoare() {}

    virtual void put(T item) {
        monitorIn();

        // Attendre qu'il y ait de la place dans le tampon
        if (nbElement == bufferSize)
            wait(notFull);

        // Ajouter un élément au tampon
        buffer[writePointer].data = item;
        buffer[writePointer].consummed = 0;
        writePointer = (writePointer + 1) % bufferSize;
        nbElement++;

        // Signaler qu'un élément est disponible pour les consommateurs
        for(int i = 0; i < NB_CONSO; i++)
            signal(notEmpty);

        monitorOut();
    }

    virtual T get() {
        monitorIn();

        // Attendre qu'un élément soit disponible
        if (nbElement == 0)
            wait(notEmpty);

        // Récupérer l'élément
        T item = buffer[readPointer].data;
        buffer[readPointer].consummed++;

        // Si l'élément a été consommé par tous les consommateurs, avancer le pointeur de lecture
        if (buffer[readPointer].consummed == NB_CONSO) {
            readPointer = (readPointer + 1) % bufferSize;
            nbElement--;

            // Signaler qu'il y a de la place pour le producteur
            signal(notFull);
        }

        monitorOut();
        return item;
    }
};




#endif // BUFFER2CONSO_H
