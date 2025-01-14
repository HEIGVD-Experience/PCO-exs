#ifndef LOADER_H
#define LOADER_H

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>

#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

class Data
{
public:
    QVector<QString> words;
};

class ILoader
{
public:
    virtual Data* getData() = 0;
};

class SimpleLoader : public ILoader
{
private:
    QString fileName;

public:
    SimpleLoader(QString filename): fileName(std::move(filename)) {}

    Data* getData() override
    {
        Data* data = new Data();
        QFile inputFile(fileName);
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                PcoThread::usleep(10);
                data->words.append(line);
            }
            inputFile.close();
        }
        return data;
    }
};

class SimpleLoaderFake : public ILoader
{
private:
    QString fileName;

public:
    SimpleLoaderFake(QString filename): fileName(std::move(filename)) {}

    Data* getData() override
    {
        Data* data = new Data();
        for (int i = 0; i < 1000; i++) {
            QString line = QString("%1").arg(i);
            PcoThread::usleep(10);
            data->words.append(line);
        }
        return data;
    }
};

class ThreadedLoader : public ILoader
{
    ILoader* adaptee;

    Data *data = nullptr;
    bool isReady = false;
    bool startLoading {};
    PcoThread *workerThread;
    PcoConditionVariable condition;

    void loading() {
        data = adaptee->getData();
        isReady = true;
        //signal conditions
    }

    bool isReady() {
        return data != nullptr;
    }

    void startPreloadingInternal() {
        if(!startLoading) {
            startLoading = true;
            workerThread = new PcoThread(&ThreadedLoader::loading, this);
        }
    }

    void workerThread() {

        data = adaptee->getData();
        monitorIn();
        isReady = true;
        signal(condition);
        monitorOut();
    }

public:
    ThreadedLoader(ILoader* adaptee): adaptee(adaptee)
    {

    }

    ~ThreadedLoader()
    {
        if(workerThread != nullptr) {
            workerThread->join();
            delete workerThread;
        }
    }

    void startPreloading()
    {
         monitorIn();
         startPreloadingInternal();
         monitorOut();

    }
    Data *getData() override {

      monitorIn();
      startPreloadingInternal();
        if(!isReady) {
           wait(condition);
           signal(condition);
        }
        monitorOut();
        return data;

    }
};
#endif // LOADER_H
