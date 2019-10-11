#include <iostream>
#include "../../include/Map.h"
#include "../../include/MapLoader.h"

int main() {
    //init loader
    const std::string MAP1 = "world.map";
    MapLoader myLoader = MapLoader(MAP1);

    //create first (valid) map
    std::cout << "creating map 1 (valid)\n";
    Map* firstMap = myLoader.readMapFile();
    std::cout << "map 1 creation completed.\n";
    if (firstMap) {
        firstMap->printMap();
    }
    
    //create second (invalid) map
    /*
     * this map is invalid in many ways. Alaska is assigned to a continent
     * that doesnt exist, alberta has a wrong country ID and it is not a
     * connected graph (that can be tested for in Map.cpp).
     */
    std::cout << "\n\n";
    const std::string MAP2 = "world2.map";
    myLoader.setMapFile(MAP2);
    std::cout << "\ncreating map 2 (invalid)\n";
    Map* secondMap = myLoader.readMapFile();
    std::cout << "map 2 creation attempt completed.\n";
    if (secondMap) {
        secondMap->printMap();
    }
    return 0;
}