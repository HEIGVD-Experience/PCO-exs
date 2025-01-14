cmake_minimum_required(VERSION 3.13)

project(preloader_tests)

find_package(Qt5 COMPONENTS Core)
if (NOT Qt5_FOUND)
    find_package(Qt6 COMPONENTS Core REQUIRED)
endif()

if (Qt5_FOUND)
    add_definitions(-DUSING_QT5)
else()
    add_definitions(-DUSING_QT6)
endif()

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/loader.h
)

add_executable(preloader_tests ${SOURCES} ${HEADERS})

if (Qt5_FOUND)
    target_link_libraries(preloader_tests PRIVATE Qt5::Core -lpcosynchro gtest)
else()
    target_link_libraries(preloader_tests PRIVATE Qt6::Core -lpcosynchro gtest)
endif()
