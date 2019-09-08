cmake_minimum_required(VERSION 3.14)
project(COMP-345-proj)

set(CMAKE_CXX_STANDARD 14)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mapconfig.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(COMP-345-proj main.cpp map.cpp)