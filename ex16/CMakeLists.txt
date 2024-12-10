cmake_minimum_required(VERSION 3.13)

project(producerconsumermultiple2_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        prodcons1.cpp
)

set(HEADERS
        abstractbuffer.h
        buffer2conso.h
)

add_executable(producerconsumermultiple2_tests ${SOURCES} ${HEADERS})

target_link_libraries(producerconsumermultiple2_tests PRIVATE -lpcosynchro gtest)

