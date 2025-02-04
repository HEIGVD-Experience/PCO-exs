
#include <iostream>

#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcothread.h>

#include "semaphorefrommonitor.h"
#include "pcosynchro/pcotest.h"

template<typename T>
class SemaphoreFromMonitor : public ::testing::Test
{

protected:


    void asMutex() {
        // Req: A critical section protected by a PcoMutex should not be reached
        //      by more that one thread
        ASSERT_DURATION_LE(10,{
        T mutex(1);
        bool oneIn = false;
        std::thread t1([&oneIn, &mutex](){
            mutex.wait();
            ASSERT_EQ(oneIn, false);
            oneIn = true;
            std::chrono::microseconds value(1000);
            std::this_thread::sleep_for(value);
            ASSERT_EQ(oneIn, true);
            oneIn = false;
            std::chrono::microseconds value1(1000);
            std::this_thread::sleep_for(value1);
            ASSERT_EQ(oneIn, false);
            mutex.post();
        });

        std::thread t2([&oneIn, &mutex](){
            mutex.wait();
            ASSERT_EQ(oneIn, false);
            oneIn = true;
            std::chrono::microseconds value(500);
            std::this_thread::sleep_for(value);
            ASSERT_EQ(oneIn, true);
            oneIn = false;
            std::chrono::microseconds value1(500);
            std::this_thread::sleep_for(value1);
            ASSERT_EQ(oneIn, false);
            mutex.post();
        });
        t1.join();
        t2.join();
        })
    }


    void tripleWait() {
        // Req: A semaphore initialized to 0 should block 3 threads
        //      that call wait(), and then these threads should be
        //      awaken by three post().
        ASSERT_DURATION_LE(10,{
            T sem(0);
            bool oneIn = false;
            std::thread t1([&oneIn, &sem](){
                sem.wait();
            });

            std::thread t2([&oneIn, &sem](){
                sem.wait();
            });

            std::thread t3([&oneIn, &sem](){
                sem.wait();
            });
            PcoThread::usleep(1000000);
            sem.post();
            PcoThread::usleep(10000);
            sem.post();
            sem.post();
            t1.join();
            t2.join();
            t3.join();
        })
    }

    void blocking() {
        // Req: A semaphore that reaches a negative value blocks the caller

        // We block directly at least one thread
        ASSERT_DURATION_GE(1,T sem(0);sem.wait())
        ASSERT_DURATION_GE(1,T sem(1);sem.wait();sem.wait())
        ASSERT_DURATION_GE(1,T sem(2);sem.wait();sem.wait();sem.wait())
    }

    void nonBlocking() {
    // Req: A semaphore that has a positive value should not block the caller

    // We do not block any thread
    ASSERT_DURATION_LE(1,{T sem(1);sem.wait();})
    ASSERT_DURATION_LE(1,T sem(2);sem.wait();sem.wait())
    ASSERT_DURATION_LE(1,T sem(3);sem.wait();sem.wait();sem.wait())
    }

    void testTryWait() {
        ASSERT_DURATION_LE(1, {
        T sem(2);
        bool result;
        result = sem.trywait();
        ASSERT_EQ(result, true);
        result = sem.trywait();
        ASSERT_EQ(result, true);
        result = sem.trywait();
        ASSERT_EQ(result, false);
                           })
    }
};

using MyTypes = ::testing::Types<SemaphoreFromMesaMonitor, SemaphoreFromHoareMonitor>;

TYPED_TEST_SUITE(SemaphoreFromMonitor, MyTypes);

TYPED_TEST(SemaphoreFromMonitor, NonBlocking) {
    this->nonBlocking();
}

TYPED_TEST(SemaphoreFromMonitor, blocking) {
    this->blocking();
}

TYPED_TEST(SemaphoreFromMonitor, asMutex) {
    this->asMutex();
}

TYPED_TEST(SemaphoreFromMonitor, testTryWait) {
    this->testTryWait();
}

TYPED_TEST(SemaphoreFromMonitor, testTripleWait) {
    this->tripleWait();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
