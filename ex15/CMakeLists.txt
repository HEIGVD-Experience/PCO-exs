cmake_minimum_required(VERSION 3.13)

project(producerconsumersimple2_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/prodcons1.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractbuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer2conso.h
)

add_executable(producerconsumersimple2_tests ${SOURCES} ${HEADERS})

target_link_libraries(producerconsumersimple2_tests PRIVATE -lpcosynchro gtest)

