#include <iostream>
#include "map.h"
#include <cmath>
#include <fstream>

int main() 
{
    //read vertices from file
    std::vector<std::vector <int>> map_vertices = getVertices("mapconfig.txt");
    //display them
    printVertices(map_vertices);

    return 0;
}