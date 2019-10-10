cmake_minimum_required(VERSION 3.14)
project(COMP-345-proj)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-unknown-pragmas
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/world.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/world2.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(COMP-345-proj main.cpp src/Cards.cpp src/Dice.cpp include/Cards.h include/Dice.h src/Drivers/CardsDriver.cpp src/Drivers/DiceDriver.cpp)
add_executable(Cards-driver src/Cards.cpp include/Cards.h src/Drivers/CardsDriver.cpp)
add_executable(Dice-driver src/Dice.cpp include/Dice.h src/Drivers/DiceDriver.cpp)
add_executable(map-driver src/Drivers/MapDriver.cpp src/Map.cpp include/Map.h)
add_executable(map-loader-driver src/Drivers/MapLoaderDriver.cpp src/Map.cpp include/Map.h src/MapLoader.cpp include/MapLoader.h)
