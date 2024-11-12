cmake_minimum_required(VERSION 3.13)

project(pcobarrier_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/pcobarrier.h
)

add_executable(pcobarrier_tests ${SOURCES} ${HEADERS})

target_link_libraries(pcobarrier_tests PRIVATE -lpcosynchro gtest)

