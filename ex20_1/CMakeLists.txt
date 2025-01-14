cmake_minimum_required(VERSION 3.13)

project(lectred_priored_tests)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/readerwriter.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractreaderwriter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterpriowritergeneral.h
)

add_executable(lectred_priored_tests ${SOURCES} ${HEADERS})

target_link_libraries(lectred_priored_tests PRIVATE -lpcosynchro gtest)

