cmake_minimum_required(VERSION 3.13)

project(bridgefloat_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bridgemanagerfloat.h
)

add_executable(bridgefloat_tests ${SOURCES} ${HEADERS})

target_link_libraries(bridgefloat_tests PRIVATE -lpcosynchro gtest)

