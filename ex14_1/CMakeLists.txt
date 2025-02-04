cmake_minimum_required(VERSION 3.13)

project(bridgefloatstop_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bridgemanagerfloat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vehicle.h
)

add_executable(bridgefloatstop_tests ${SOURCES} ${HEADERS})

target_link_libraries(bridgefloatstop_tests PRIVATE -lpcosynchro gtest)

