cmake_minimum_required(VERSION 3.13)

project(bridgesimple_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bridgemanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bridgemanagermonitor.h
)

add_executable(bridgesimple_tests ${SOURCES} ${HEADERS})

target_link_libraries(bridgesimple_tests PRIVATE -lpcosynchro gtest)

