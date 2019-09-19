cmake_minimum_required(VERSION 3.14)
project(COMP-345-proj)

set(CMAKE_CXX_STANDARD 14)

add_executable(COMP-345-proj main.cpp src/Cards.cpp src/Dice.cpp include/Cards.h include/Dice.h src/Drivers/CardsDriver.cpp src/Drivers/DiceDriver.cpp)
add_executable(Cards-driver src/Cards.cpp include/Cards.h src/Drivers/CardsDriver.cpp)
add_executable(Dice-driver src/Dice.cpp include/Dice.h src/Drivers/DiceDriver.cpp)
