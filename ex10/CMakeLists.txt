cmake_minimum_required(VERSION 3.13)

project(mutexfromsemaphore_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/mutexfromsem.h
)

add_executable(mutexfromsemaphore_tests ${SOURCES} ${HEADERS})

target_link_libraries(mutexfromsemaphore_tests PRIVATE -lpcosynchro gtest)

