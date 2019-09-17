//
// Created by lebou on 2019-09-15.
//
#include <iostream>
#include <vector>
#include <string>
#include "../include/Map.h"
#ifndef COMP_345_PROJ_MAPLOADER_H
#define COMP_345_PROJ_MAPLOADER_H

class MapLoader{
public:
    MapLoader(std::string mapFile);
    std::string* pMapFile;
    void setMapFile(std::string newMapFile);
    Map* initMap();

private:
    static Map* initMapObject(std::string mn, std::vector<std::vector<std::string>> ctd, std::vector<std::vector<std::string>> cyd, std::vector<std::vector<int>> bd, BOOLEAN vMap);
};

#endif //COMP_345_PROJ_MAPLOADER_H


