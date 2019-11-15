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

AlternativeLoader::AlternativeLoader(std::string mapFile) {
    pDominationMapFile = new std::string(std::move(mapFile));
}

AlternativeLoader::~AlternativeLoader() {
    delete pDominationMapFile;
}

AlternativeLoader::AlternativeLoader(const AlternativeLoader &toCopy) {
    pDominationMapFile = new std::string();
    *pDominationMapFile = *toCopy.pDominationMapFile;
}

void AlternativeLoader::operator=(AlternativeLoader &rhs) {
    this->pDominationMapFile = rhs.pDominationMapFile;
}

void AlternativeLoader::altSetMapFile(std::string newMapFile) {
    pDominationMapFile = &newMapFile;
}

Map *AlternativeLoader::altReadMapFile() {
    //create file stream to read file line by line
    std::ifstream infile(*pDominationMapFile);
    if(!infile || infile.peek() == EOF){
        std::cout << *pDominationMapFile;
        return nullptr;
    }
    std::string line;

    //declare map info
    auto* pMapName = new std::string;
    auto* pContinentData = new std::vector<std::vector<std::string>>;
    auto* pTerritoryData = new std::vector<std::vector<std::string>>;
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
            //check if we enter a new section
            if (altCheckSection(pMode, line)) {
                continue;
            }
            altSplitLine(line, pLineWords, *pMode);
            altGetMapName(pMapName, pLineWords);

            //read sections
            if (*pMode == "Map") {
                //this mode is in every example file, do we need it?
                std::cout << "Line " << *pLineCount
                          << " - [WARNING] : the parser encountered the Map mode, which is not supported at the moment.\n";
            } else if (*pMode == "Continents") {
                if (altValidateContinentLine(pContinentCount, pLineWords, pLineCount, pValidMap)) {
                    pContinentData->push_back(*pLineWords);
                } else {
                    return nullptr;
                }
            } else if (*pMode == "Territories") {
                if (altValidateTerritoriesLine(pLineWords, pLineCount, pValidMap)) {
                    pTerritoryData->push_back(*pLineWords);
                } else {
                    return nullptr;
                }
            } else {
                //unknown mode error, will be ignored, non-critical
                std::cout << "\nLine " << *pLineCount << " - [WARNING] : the parser encountered an unknown mode. ";
                std::cout << " ::  " << line;
            }
        }
        delete (pLineWords);
    }
    //create map object
    auto* map = altInitMapObject(pMapName, pContinentData, pTerritoryData, pValidMap);

    //clean up
    delete (pMode);
    delete (pLineCount);
    delete (pContinentCount);
    delete (pValidMap);
    delete (pContinentData);
    delete (pCountryCount);
    delete (pTerritoryData);
    delete (pCountryID);

    return map;
}

bool AlternativeLoader::altCheckSection(std::string *mode, const std::string &line) {
    //separate in sections
    if (line[0] == *"[") {
        *mode = "";
        for (unsigned long i = 1; i < line.size() - 1; i++) {
            *mode += line[i];
        }
        return true;
    }
    return false;
}

Map *AlternativeLoader::altInitMapObject(std::string *mapName, std::vector<std::vector<std::string>> *continentData,
                                         std::vector<std::vector<std::string>> *territoryData, const bool *vMap) {
    if (*vMap) {
        //create map object with empty continents
        Map* gameMap = new Map(*mapName, *continentData);
        //add countries to map and continents
        std::vector<std::vector<std::string>> countryData;
        countryData.resize(territoryData->size());
        std::vector<std::vector<int>> borderData;
        borderData.resize(territoryData->size());
        std::vector<std::string> countryList;
        unsigned int counter = 0;
        //populate countryData and maintain a countryList to figure out country IDs
        for(auto& i : *territoryData){
            countryData[counter].push_back(std::to_string(counter + 1));
            countryData[counter].push_back(i[0]);
            if(getContinentID(i[3],*continentData) == "0"){
                return nullptr;
            }
            countryData[counter].push_back(getContinentID(i[3],*continentData));
            countryList.push_back(i[0]);
            counter++;
        }
        //populate border data
        counter = 0;
        for(auto& i : *territoryData){
            borderData[counter].push_back(counter + 1);
            for(unsigned int j = 4; j < territoryData->size(); j++){
                if(getCountryID(i[j],countryList) == 0){
                    return nullptr;
                }
                borderData[counter].push_back(getCountryID(i[j],countryList));
            }
            counter++;
        }

        //add countries to map
        for (auto& i : countryData) {
            gameMap->addNode(std::stoi(i[0]), i[1], std::stoi(i[2]));
        }

        //add adjacency
        for (auto& i : borderData) {
            for (unsigned long j = 1; j < i.size(); j++) {
                gameMap->addEdge(i[0], i[j]);
            }
        }
        return gameMap;
    }
    //there was an error, map was not created
    return nullptr;
}

void AlternativeLoader::altSplitLine(const std::string &line, std::vector<std::string> *pLineWords, const std::string& mode) {
    std::string mLine = line;
    size_t pos;
    std::string token;
    std::string delimiter;
    if(mode == "Map" || mode == "Continents"){
        //split each line into vector of words (split by =)
        delimiter = "=";
        while((pos = mLine.find(delimiter)) != std::string::npos){
            token = mLine.substr(0,pos);
            pLineWords->push_back(token);
            mLine.erase(0,pos + delimiter.length());
        }
    }else{
        //split each line into vector of words (split by ,)
        delimiter = ",";
        while((pos = mLine.find(delimiter)) != std::string::npos){
            token = mLine.substr(0,pos);
            pLineWords->push_back(token);
            mLine.erase(0,pos + delimiter.length());
        }
    }
}

void AlternativeLoader::altGetMapName(std::string *mapName, std::vector<std::string> *lineWords) {
    //get map name
    if (lineWords->at(0) == "name" || lineWords->at(0) == "Name") {
        for (unsigned long i = 1; i < lineWords->size(); i++) {
            *mapName += (*lineWords)[i] + " ";
        }
    }
}

bool
AlternativeLoader::altValidateContinentLine(int *continentCount, std::vector<std::string> *lineWords, const int *lineCount,
                                            bool *validMap) {
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

bool
AlternativeLoader::altValidateTerritoriesLine(std::vector<std::string> *lineWords, const int *lineCount,
                                              bool *validMap) {
    //check validity of the line in this mode
    if (lineWords->size() < 5) {
        std::cout << "Line " << *lineCount
                  << " - [ERROR] : a line in the borders declaration had missing tokens, map could not be created.\n";
        *validMap = false;
        return false;
    }
    return true;
}

std::string AlternativeLoader::getContinentID(const std::string& continentName,std::vector<std::vector<std::string>> continentData) {
    unsigned int counter = 0;
    for(auto& i : continentData){
        if(i[0] == continentName){
            return std::to_string(counter + 1);
        }
        counter++;
    }
    return std::to_string(0);
}

int AlternativeLoader::getCountryID(const std::string& countryName, const std::vector<std::string>& countryList) {
    int counter = 0;
    for(auto& i : countryList){
        if(i == countryName){
            return counter + 1;
        }
        counter++;
    }
    return 0;
}
