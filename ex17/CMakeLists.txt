cmake_minimum_required(VERSION 3.13)

project(semaphorefrommonitor_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/semaphorefrommonitor.h
)

add_executable(semaphorefrommonitor_tests ${SOURCES} ${HEADERS})

target_link_libraries(semaphorefrommonitor_tests PRIVATE -lpcosynchro gtest)

