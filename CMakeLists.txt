cmake_minimum_required(VERSION 3.14)
project(COMP-345-proj)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(
        -Wall
        -Wextra
        #-Werror
        -Wno-unknown-pragmas
        -Wno-unused-function
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/world.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/world2.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/worldEmpty.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(Cards-driver)
add_executable(Dice-driver)
add_executable(Map-driver)
add_executable(Map-Loader-driver)
add_executable(Player-driver)
add_executable(Game-engine-driver)

target_include_directories(Cards-driver PRIVATE "include")
target_sources(Cards-driver PRIVATE "src/Cards.cpp" "src/Drivers/CardsDriver.cpp")

target_include_directories(Dice-driver PRIVATE "include")
target_sources(Dice-driver PRIVATE "src/Dice.cpp" "src/Drivers/DiceDriver.cpp")

target_include_directories(Map-driver PRIVATE "include")
target_sources(Map-driver PRIVATE "src/Map.cpp" "src/Drivers/MapDriver.cpp")

target_include_directories(Map-Loader-driver PRIVATE "include")
target_sources(Map-Loader-driver PRIVATE "src/MapLoader.cpp" "src/Drivers/MapLoaderDriver.cpp" "src/Map.cpp")

target_include_directories(Player-driver PRIVATE "include")
target_sources(Player-driver PRIVATE "src/Player.cpp" "src/Drivers/PlayerDriver.cpp" "src/Map.cpp" "src/Dice.cpp" "src/Cards.cpp")

target_include_directories(Game-engine-driver PRIVATE "include")
target_sources(Game-engine-driver PRIVATE "src/Drivers/GameEngineDriver.cpp" "src/Player.cpp" "src/GameEngine.cpp" "src/Map.cpp" "src/Dice.cpp" "src/Cards.cpp" "src/MapLoader.cpp")

