
#include <iostream>

#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcothread.h>

#include "mutexfromsem.h"

static const int NB_THREADS = 10;
static const int NB_ITERATIONS = 200000;

class MutexFromSemTest : public ::testing::Test
{
public:
    int global{0};
    MutexFromSem mutex;

    void runTask()
    {
        for(int i = 0; i < NB_ITERATIONS; i++) {
            mutex.lock();
            global = global + 1;
            mutex.unlock();
        }
    }
};

TEST_F(MutexFromSemTest, easy) {
    global = 0;
    std::vector<std::unique_ptr<PcoThread>> threads(NB_THREADS);
    for(int i = 0; i < NB_THREADS; i++) {
        threads[i] = std::make_unique<PcoThread>(&MutexFromSemTest::runTask, this);
    }
    for(int i = 0; i < NB_THREADS; i++) {
        threads[i]->join();
    }
    logger() << "Fin des taches : global = " << global << " (" << NB_THREADS * NB_ITERATIONS << ")" << '\n';
    ASSERT_EQ(global, NB_THREADS * NB_ITERATIONS);
}

// Simplistic test to check if trylock() is somewhat behaving as expected
// TODO concurrent implementation to detect unsafe trylock()
TEST_F(MutexFromSemTest, trylock) {
    MutexFromSem mutex;
    bool result;

    // lock (-> ok) + trylock (-> cannot lock)
    mutex.lock();
    result = mutex.trylock();
    ASSERT_EQ(result, false);
    mutex.unlock();

    // trylock (-> ok) + trylock (-> cannot lock)
    result = mutex.trylock();
    ASSERT_EQ(result, true);
    result = mutex.trylock();
    ASSERT_EQ(result, false);
    mutex.unlock();

    // "dummy test" to check that the previous double trylock did not deadlocked
    result = mutex.trylock();
    ASSERT_EQ(result, true);
}

// Simplistic test to check if extra unlocks are not causing issues
// (which would be the case if using a simple semaphor)
// Doing this test concurrently will not work because we are not keeping
// track of the thread which is owning the mutex, so we are using trylock()
// Downside: it will fail if the trylock() implementation is incorrect
TEST_F(MutexFromSemTest, mutexSemantic) {
    MutexFromSem mutex;
    bool result;

    // regular lock + unlock
    mutex.lock();
    mutex.unlock();
    // some extra unlocks
    mutex.unlock();
    mutex.unlock();
    mutex.unlock();

    // lock (-> ok) + trylock (-> cannot lock)
    mutex.lock();
    result = mutex.trylock();
    ASSERT_EQ(result, false);
    mutex.unlock();
    // some extra unlocks
    mutex.unlock();
    mutex.unlock();
    mutex.unlock();

    // trylock (-> ok) + trylock (-> cannot lock)
    result = mutex.trylock();
    ASSERT_EQ(result, true);
    result = mutex.trylock();
    ASSERT_EQ(result, false);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
