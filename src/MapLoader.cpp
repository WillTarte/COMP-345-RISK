//
// Created by lebou on 2019-09-15.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStatic"

#include "../include/Map.h"
#include <sstream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <iostream>
#include "../include/MapLoader.h"

/**
 * MapLoader constructor
 * @param mapFile
 */
MapLoader::MapLoader(std::string mapFile) {
    pMapFile = new std::string(std::move(mapFile));
}

/**
 * Map loader copy constructor
 * @param toCopy
 */
MapLoader::MapLoader(const MapLoader &toCopy) {
    pMapFile = new std::string();
    *pMapFile = *toCopy.pMapFile;
}

/**
 * MapLoader destructor
 */
MapLoader::~MapLoader() {
    delete pMapFile;
}

/**
 * assignment operator
 */
void MapLoader::operator=(MapLoader& rhs) {
    this->pMapFile = rhs.pMapFile;
}

//TODO: For all valid lines in world map files, strip new lines (\n) and carriage returns (\r)
Map* MapLoader::readMapFile() {
    //create file stream to read file line by line
    std::ifstream infile(*pMapFile);
    if(!infile || infile.peek() == EOF){
        return nullptr;
    }
    std::string line;

    //declare map info
    auto* pMapName = new std::string;
    auto* pContinentData = new std::vector<std::vector<std::string>>;
    auto* pCountryData = new std::vector<std::vector<std::string>>;
    auto* pBorderData = new std::vector<std::vector<int>>;
    auto* pMode = new std::string;

    //maintain counters for validation / error tracing
    auto* pLineCount = new int(0);
    auto* pCountryID = new int(1);
    auto* pContinentCount = new int(0);
    auto* pCountryCount = new int(0);
    auto* pValidMap = new bool(true);

    while (std::getline(infile, line)) {
        (*pLineCount)++;
        //will hold the words of each line (separated by spaces) in an array
        auto* pLineWords = new std::vector<std::string>;
        //ignore comments or empty lines
        if (line.empty() || line[0] == *";" || line[0] == *"\n" || line[0] == *"\r") {
            //do nothing, the line is empty or is a comment
            continue;
        } else {
            splitLine(line, pLineWords);
            getMapName(pMapName, pLineWords);

            if (checkSection(pMode, pLineWords)) {
                continue;
            }

            //read sections
            if (*pMode == "files") {
                //this mode is in every example file, do we need it?
                std::cout << "Line " << *pLineCount
                          << " - [WARNING] : the parser encountered the file mode, which is not supported at the moment.\n";
            } else if (*pMode == "continents") {
                if (validateContinentLine(pContinentCount, pLineWords, pLineCount, pValidMap)) {
                    pContinentData->push_back(*pLineWords);
                } else {
                    return nullptr;
                }
            } else if (*pMode == "countries") {
                if (validateCountryLine(pCountryCount, pLineWords, pLineCount, pValidMap, pCountryID,
                                        pContinentCount)) {
                    pCountryData->push_back(*pLineWords);
                } else {
                    return nullptr;
                }
            } else if (*pMode == "borders") {
                auto* pLineNums = new std::vector<int>;
                if (validateBordersLine(pLineNums, pLineWords, pLineCount, pValidMap, pCountryCount)) {
                    pBorderData->push_back(*pLineNums);
                } else {
                    return nullptr;
                }
            } else if (pLineWords->at(0) == "name") {
                continue;
            } else {
                //unknown mode error, will be ignored, non-critical
                std::cout << "\nLine " << *pLineCount << " - [WARNING] : the parser encountered an unknown mode. ";
                std::cout << " ::  " << line;
            }
        }
        delete (pLineWords);
    }
    //create map object
    auto* map = initMapObject(pMapName, pContinentData, pCountryData, pBorderData, pValidMap);

    //clean up
    delete (pMode);
    delete (pLineCount);
    delete (pContinentCount);
    delete (pValidMap);
    delete (pContinentData);
    delete (pCountryCount);
    delete (pCountryData);
    delete (pCountryID);
    delete (pBorderData);

    return map;
}

Map* MapLoader::initMapObject(std::string* mapName, std::vector<std::vector<std::string>>* continentData,
                              std::vector<std::vector<std::string>>* countryData,
                              std::vector<std::vector<int>>* borderData, const bool* vMap) {
    if (*vMap) {
        //create map object with empty continents
        Map* gameMap = new Map(*mapName, *continentData);
        //add countries to map and continents
        for (auto& i : *countryData) {
            gameMap->addNode(std::stoi(i[0]), i[1], std::stoi(i[2]));
        }

        //add adjacency
        for (auto& i : *borderData) {
            for (unsigned long j = 1; j < i.size(); j++) {
                gameMap->addEdge(i[0], i[j]);
            }
        }
        return gameMap;
    }
    //there was an error, map was not created
    return nullptr;
}

void MapLoader::setMapFile(std::string newMapFile) {
    pMapFile = &newMapFile;
}

void MapLoader::splitLine(const std::string& line, std::vector<std::string>* pLineWords) {
    //split each line into vector of words (split by spaces)
    std::istringstream iss(line);
    for (std::string s; iss >> s;) {
        pLineWords->push_back(s);
    }
}

void MapLoader::getMapName(std::string* mapName, std::vector<std::string>* lineWords) {
    //get map name
    if (lineWords->at(0) == "name" || lineWords->at(0) == "Name") {
        for (unsigned long i = 1; i < lineWords->size(); i++) {
            *mapName += (*lineWords)[i] + " ";
        }
    }
}

bool MapLoader::checkSection(std::string* mode, std::vector<std::string>* lineWords) {
    //separate in sections
    if ((*lineWords)[0][0] == *"[") {
        *mode = "";
        for (unsigned long i = 1; i < (*lineWords)[0].size() - 1; i++) {
            *mode += (*lineWords)[0][i];
        }
        return true;
    }
    return false;
}

bool MapLoader::validateContinentLine(int* continentCount, std::vector<std::string>* lineWords, const int* lineCount,
                                      bool* validMap) {
    (*continentCount)++;
    //check validity of the line in this mode
    if (lineWords->size() < 2) {
        std::cout << "Line " << *lineCount
                  << " - [ERROR] : a line in the continents declaration had missing tokens, map could not be created.\n";
        *validMap = false;
        return false;
    } else if (lineWords->size() > 2) {
        std::cout << "Line " << *lineCount
                  << " - [WARNING] : a line in  the continents declaration had extra tokens.\n";
        return true;
    } else {
        return true;
    }
}

bool MapLoader::validateCountryLine(int* countryCount, std::vector<std::string>* lineWords, const int* lineCount,
                               bool* validMap,
                               int* countryID, const int* continentCount) {
    (*countryCount)++;
    //check validity of the line in this mode
    if (lineWords->size() < 3) {
        std::cout << "Line " << *lineCount
                  << " - [ERROR] : a line in the countries declaration had missing tokens, map could not be created.\n";
        *validMap = false;
        return false;
    } else {
        if (lineWords->size() > 3)
            std::cout << "Line " << *lineCount
                      << " - [WARNING] : a line in  the countries declaration had extra tokens.\n";
        //check country id matches with order and that it references a valid continent
        if (std::stoi((*lineWords)[0]) == *countryID && std::stoi((*lineWords)[2]) <= *continentCount) {
            (*countryID)++;
            return true;
        } else {
            std::cout << "Line " << *lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
            *validMap = false;
            return false;
        }
    }
}

bool MapLoader::validateBordersLine(std::vector<int>* lineNums, std::vector<std::string>* lineWords, const int* lineCount,
                               bool* validMap, const int* countryCount) {
    //check validity of the line in this mode
    if (lineWords->size() < 2) {
        std::cout << "Line " << *lineCount
                  << " - [ERROR] : a line in the borders declaration had missing tokens, map could not be created.\n";
        *validMap = false;
        return false;
    } else {
        //convert strings to ints
        for (auto& s : *lineWords) {
            std::stringstream parser(s);
            int x = 0;
            parser >> x;
            lineNums->push_back(x);
            //check that all countries referenced in this line exist
            if (x > *countryCount) {
                std::cout << "Line " << *lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
                *validMap = false;
                return false;
            }
        }
        return true;
    }
}
