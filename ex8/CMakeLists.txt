cmake_minimum_required(VERSION 3.13)

project(sequentialite3_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(sequentialite3_tests ${SOURCES})

target_link_libraries(sequentialite3_tests PRIVATE -lpcosynchro gtest)

