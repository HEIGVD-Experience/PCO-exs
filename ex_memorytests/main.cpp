#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <vector>


using namespace std;

// If not volatile, clang will discard its accesses
static volatile int sharedVariable = 0;

void* tFunction(void *arg)
{
    int id = (intptr_t)(arg);
    sharedVariable = id;
    return nullptr;
}

void testBadMultiThread()
{
    pthread_t thread1;
    pthread_create(&thread1, nullptr, tFunction, nullptr);
    pthread_t thread2;
    pthread_create(&thread2, nullptr, tFunction, nullptr);
}

void testMultiThread()
{
    pthread_t thread1;
    pthread_create(&thread1, nullptr, tFunction, nullptr);
    pthread_t thread2;
    pthread_create(&thread2, nullptr, tFunction, nullptr);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
}

static pthread_mutex_t mutex;

void* tBetterFunction(void *arg)
{
    int id = (intptr_t)(arg);
    pthread_mutex_lock(&mutex);
    sharedVariable = id;
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

void testBetterMultiThread()
{
    pthread_t thread1;
    pthread_create(&thread1, nullptr, tBetterFunction, nullptr);
    pthread_t thread2;
    pthread_create(&thread2, nullptr, tBetterFunction, nullptr);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
}


int main(int argc, char **argv) {
    int testCase = 0;
    if (argc == 2) {
        testCase = atoi(argv[1]);
    }

    switch (testCase) {
    case 1: testBadMultiThread(); break;
    case 2: testMultiThread(); break;
    case 3: testBetterMultiThread(); break;
    default: std::cout << "Test " << testCase << " not implemented" << std::endl;
    }

    return 0;
}

