cmake_minimum_required(VERSION 3.14)
project(COMP-345-proj)

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17)
# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
        -Wall
        -Wextra
        -Wno-unknown-pragmas
        -Wno-unused-function
        -lstdc++fs
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/references/")
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/
#        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/world.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/world2.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/worldEmpty.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/references/otherWorkingMap.map
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(Cards-driver "src/Drivers/CardsDriver.cpp")
add_executable(Test-Driver "src/Drivers/TestDriver.cpp")
add_executable(Dice-driver "src/Drivers/DiceDriver.cpp")
add_executable(Map-driver "src/Drivers/MapDriver.cpp")
add_executable(Map-Loader-driver "src/Drivers/MapLoaderDriver.cpp")
add_executable(Player-driver "src/Drivers/PlayerDriver.cpp")
add_executable(Game-engine-driver "src/Drivers/GameEngineDriver.cpp")

target_include_directories(Cards-driver PRIVATE "include")
target_sources(Cards-driver PRIVATE "src/Cards.cpp" "src/Drivers/CardsDriver.cpp")

target_include_directories(Dice-driver PRIVATE "include")
target_sources(Dice-driver PRIVATE "src/Dice.cpp" "src/Drivers/DiceDriver.cpp")

target_include_directories(Map-driver PRIVATE "include")
target_sources(Map-driver PRIVATE "src/Map.cpp" "src/Drivers/MapDriver.cpp")

target_include_directories(Map-Loader-driver PRIVATE "include")
target_sources(Map-Loader-driver PRIVATE "src/MapLoader.cpp" "src/Drivers/MapLoaderDriver.cpp" "src/Map.cpp")

target_include_directories(Player-driver PRIVATE "include")
target_sources(Player-driver PRIVATE "src/MapLoader.cpp" "src/Map.cpp" "src/Dice.cpp" "src/Cards.cpp" "src/GameEngine.cpp" "src/Player.cpp" "src/Drivers/PlayerDriver.cpp")

target_include_directories(Game-engine-driver PRIVATE "include")
target_sources(Game-engine-driver PRIVATE "src/Drivers/GameEngineDriver.cpp" "src/Player.cpp" "src/GameEngine.cpp" "src/Map.cpp" "src/Dice.cpp" "src/Cards.cpp" "src/MapLoader.cpp")

target_include_directories(Test-Driver PRIVATE "include")
target_sources(Test-Driver PRIVATE)

target_link_libraries(Test-Driver stdc++fs)
