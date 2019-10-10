//
// Created by lebou on 2019-09-15.
//
#include <vector>
#include <string>
#include "../include/Map.h"

#ifndef COMP_345_PROJ_MAPLOADER_H
#define COMP_345_PROJ_MAPLOADER_H

class string;

class MapLoader {
public:
    explicit MapLoader(std::string mapFile);

    std::string* pMapFile;

    void setMapFile(std::string newMapFile);

    Map* readMapFile();

private:
    static Map* initMapObject(std::string* mapName, std::vector<std::vector<std::string>>* continentData,
                              std::vector<std::vector<std::string>>* countryData,
                              std::vector<std::vector<int>>* borderData, const bool* vMap);

    void splitLine(const std::string& line, std::vector<std::string>* pLineWords);

    void getMapName(std::string* mapName, std::vector<std::string>* lineWords);

    bool checkSection(std::string* mode, std::vector<std::string>* lineWords);

    bool validateContinentLine(int* continentCount, std::vector<std::string>* lineWords, const int* lineCount,
                               bool* validMap);

    bool
    validateCountryLine(int* countryCount, std::vector<std::string>* lineWords, const int* lineCount, bool* validMap,
                        int* countryID, const int* continentCount);

    bool validateBordersLine(std::vector<int>* lineNums, std::vector<std::string>* lineWords, const int* lineCount,
                             bool* validMap,
                             const int* countryCount);
};

#endif //COMP_345_PROJ_MAPLOADER_H


