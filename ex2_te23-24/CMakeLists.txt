cmake_minimum_required(VERSION 3.13)

project(RacletteTIC)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(RacletteTIC ${SOURCES})

target_link_libraries(RacletteTIC PRIVATE -lpcosynchro gtest)

