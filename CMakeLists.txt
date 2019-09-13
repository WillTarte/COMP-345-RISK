cmake_minimum_required(VERSION 3.14)
project(COMP-345-proj)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP-345-proj main.cpp src/Cards.cpp include/Cards.h src/Drivers/CardsDriver.cpp)