/** @file readerwriter.cpp
 *  @brief Test of reader-writer protocols
 *
 *  This code instantiates a reader-writer protocol and starts
 *  a certain number of readers and writers threads.
 *  A checker performs a light check in order to ensure
 *  mutual exclusion between readers and writers.
 *  You just need to change the resource creation to create
 *  various protocols.
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */

// General includes
#include <iostream>
#include <random>

#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcotest.h>

// Application related includes
#include "toilets.h"


class ToiletChecker {
public:
    ToiletChecker(int nbSeats) : nbSeats(nbSeats) {}

    void manIn()
    {
        checkerLock.lock();
        nbMen++;
        // Check after getting in
        check();
        checkerLock.unlock();

    }

    void manOut()
    {
        checkerLock.lock();
        // Check before leaving
        check();
        nbMen--;
        checkerLock.unlock();
    }

    void womanIn()
    {
        checkerLock.lock();
        nbWomen++;
        // Check after getting in
        check();
        checkerLock.unlock();
    }

    void womanOut()
    {
        checkerLock.lock();
        // Check before leaving
        check();
        nbWomen--;
        checkerLock.unlock();
    }

private:

    void check()
    {
        ASSERT_TRUE((nbWomen == 0) || (nbMen == 0));
        ASSERT_LE(nbWomen + nbMen, nbSeats);
    }

    std::mutex checkerLock;

    int nbWomen{0};
    int nbMen{0};
    int nbSeats{0};
};



void taskMan(int tid, int nbIterations, AbstractToilet *resource, ToiletChecker *checker)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 1000);

    for(int iter = 0; iter < nbIterations; iter ++) {
        resource->manAccessing();
        checker->manIn();
        logger() << "Task " << tid << ": accessing" << '\n';
        PcoThread::usleep(static_cast<uint64_t>(6.0 * dis(gen)));
        checker->manOut();
        resource->manLeaving();
        PcoThread::usleep(static_cast<uint64_t>(6.0 * dis(gen)));
    }
}


void taskWoman(int tid, int nbIterations, AbstractToilet *resource, ToiletChecker *checker)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 1000);

    for(int iter = 0; iter < nbIterations; iter ++) {
        resource->womanAccessing();
        checker->womanIn();
        logger() << "Task " << tid << ": accessing" << '\n';
        PcoThread::usleep(static_cast<uint64_t>(6.0 * dis(gen)));
        checker->womanOut();
        resource->womanLeaving();
        PcoThread::usleep(static_cast<uint64_t>(6.0 * dis(gen)));
    }
}

template<typename T>
class ToiletTest : public ::testing::Test
{
protected:

    void testToilet(int nbThreadsMen, int nbThreadsWomen, int nbSeats, int nbIterations)
    {
        ASSERT_DURATION_LE(15, {
                               std::unique_ptr<T> protocol = std::make_unique<T>(nbSeats);

                               std::vector<std::unique_ptr<std::thread> > threadsMen(nbThreadsMen);
                               std::vector<std::unique_ptr<std::thread> > threadsWomen(nbThreadsWomen);

                               auto checker = std::make_unique<ToiletChecker>(nbSeats);

                               for(int t=0; t<nbThreadsWomen; t++){
                                   logger() << "In main: creating thread Woman " << t << '\n';
                                   threadsWomen[t] = std::make_unique<std::thread>(taskWoman,
                                                                                   t,
                                                                                   nbIterations,
                                                                                   protocol.get(),
                                                                                   checker.get());
                               }
                               for(int t=0; t<nbThreadsMen; t++){
                                   logger() << "In main: creating thread Man " << t << '\n';
                                   threadsMen[t] = std::make_unique<std::thread>(taskMan,
                                                                                 t,
                                                                                 nbIterations,
                                                                                 protocol.get(),
                                                                                 checker.get());
                               }

                               for(int t=0; t<nbThreadsWomen; t++){
                                   threadsWomen[t]->join();
                               }
                               for(int t=0; t<nbThreadsMen; t++){
                                   threadsMen[t]->join();
                               }
                           });
    }
};

using MyTypes = ::testing::Types<ToiletCSemaphore>;
/*
using MyTypes = ::testing::Types<ToiletASemaphore //,
                                 // ToiletAMesa,
                                 // ToiletAHoare,
                                 // ToiletBSemaphore,
                                 // ToiletBMesa,
                                 // ToiletBHoare,
                                 // ToiletCSemaphore,
                                 // ToiletCMesa,
                                 // ToiletCHoare,
                                 // ToiletDSemaphore,
                                 // ToiletDMesa,
                                 // ToiletDHoare
                                 >;
*/

TYPED_TEST_SUITE(ToiletTest, MyTypes);

TYPED_TEST(ToiletTest, Standard) {
    this->testToilet(6, 2, 4, 1000);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
