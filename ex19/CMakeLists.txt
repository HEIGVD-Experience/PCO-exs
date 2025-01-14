cmake_minimum_required(VERSION 3.13)

project(lectred_toilettes_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/abstracttoilets.h
    ${CMAKE_CURRENT_SOURCE_DIR}/toilets.h
)

add_executable(lectred_toilettes_tests ${SOURCES} ${HEADERS})

target_link_libraries(lectred_toilettes_tests PRIVATE -lpcosynchro gtest)

