#include <iostream>
#include "../../include/Map.h"
#include "../../include/MapLoader.h"
#include <cmath>
#include <fstream>

int main()
{
    //init loader
    MapLoader myLoader = MapLoader("world.map");

    //create first (valid) map
    std::cout << "creating map 1 (valid)\n";
    Map* firstMap = myLoader.readMapFile();
    std::cout << "map 1 creation completed.\n";
    if(firstMap){
        firstMap->printMap();
    }


    //create second (invalid) map
    /*
     * this map is invalid in many ways. Alaska is assigned to a continent
     * that doesnt exist, alberta has a wrong country ID and it is not a
     * connected graph (that can be tested for in Map.cpp).
     */
    myLoader.setMapFile("world2.map");
    std::cout << "\ncreating map 2 (invalid)\n";
    Map* secondMap = myLoader.readMapFile();
    std::cout << "map 2 creation attempt completed.\n";
    if(secondMap){
        secondMap->printMap();
    }
    return 0;
}