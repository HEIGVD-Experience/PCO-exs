#include <iostream>
#include <algorithm>
#include <gtest/gtest.h>
#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcosemaphore.h>

class Checker
{
public:
    std::vector<int> executions;

    void compute(int id)
    {
        static std::mutex mutex;
        mutex.lock();
        executions.push_back(id);
        mutex.unlock();
        logger() << "Task " << id << '\n';
    }

    bool executedBefore(int id0, int id1)
    {
        auto it0 = find(executions.begin(), executions.end(), id0);
        auto it1 = find(executions.begin(), executions.end(), id1);
        return it0 < it1;
    }

    void check()
    {
        EXPECT_TRUE(executedBefore(0, 1));
        EXPECT_TRUE(executedBefore(0, 2));
        EXPECT_TRUE(executedBefore(1, 3));
        EXPECT_TRUE(executedBefore(2, 4));
        EXPECT_TRUE(executedBefore(3, 5));
        EXPECT_TRUE(executedBefore(4, 5));
    }
};

static Checker checker;


// Sémaphores pour contrôler l'ordre d'exécution
PcoSemaphore sem0(0);  // Permet à t1 et t2 de démarrer après t0
PcoSemaphore sem3(0);  // Permet à t3 de démarrer après t1
PcoSemaphore sem4(0);  // Permet à t4 de démarrer après t2
PcoSemaphore sem5(0);  // Permet à t5 de démarrer après t3 et t4

void t0()
{
    checker.compute(0);
    sem0.release();  // Libère t1 et t2
    sem0.release();  // Libère t1 et t2
}

void t1()
{
    sem0.acquire();
    checker.compute(1);
    sem3.release();   // Libère t3
}

void t2()
{
    sem0.acquire();
    checker.compute(2);
    sem4.release();   // Libère t4
}

void t3()
{
    sem3.acquire();
    checker.compute(3);
    sem5.release();   // Libère t5 (attend également t4)
}

void t4()
{
    sem4.acquire();
    checker.compute(4);
    sem5.release();   // Libère t5 (attend également t3)
}

void t5()
{
    sem5.acquire();
    sem5.acquire();  // Attendre que t3 et t4 soient terminés
    checker.compute(5);
}

TEST(Sequentialite, Standard)
{
    PcoManager::getInstance()->setMaxSleepDuration(1000, PcoManager::EventType::ThreadCreation);

    std::vector<std::unique_ptr<PcoThread>> threads(6);
    /*
        VERSION QU'AVEC DES JOIN()

    threads[0] = std::make_unique<PcoThread>(t0);

    threads[0]->join();

    threads[1] = std::make_unique<PcoThread>(t1);
    threads[2] = std::make_unique<PcoThread>(t2);

    threads[1]->join();
    threads[3] = std::make_unique<PcoThread>(t3);

    threads[2]->join();
    threads[4] = std::make_unique<PcoThread>(t4);

    threads[3]->join();
    threads[4]->join();

    threads[5] = std::make_unique<PcoThread>(t5);

    threads[5]->join();
    */

    threads[0] = std::make_unique<PcoThread>(t0);
    threads[1] = std::make_unique<PcoThread>(t1);
    threads[2] = std::make_unique<PcoThread>(t2);
    threads[3] = std::make_unique<PcoThread>(t3);
    threads[4] = std::make_unique<PcoThread>(t4);
    threads[5] = std::make_unique<PcoThread>(t5);

    for (int i = 0; i < 6; i++) {
        threads[i]->join();
    }

    checker.check();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
