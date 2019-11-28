//
// Created by lebou on 2019-09-15.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemberFunctionCanBeStatic"

#include "../include/Map.h"
#include "../include/MapLoader.h"
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <iostream>

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

/**
 * Main method of the map loader. Parses the file and creates the map object
 * @return Map
 */
Map* MapLoader::readMapFile() {
    //for debug purposes, change to true to print errors and warnings
    bool verbose = false;
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
            // strip every line of the \n and \r characters
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            splitLine(line, pLineWords);
            getMapName(pMapName, pLineWords);

            if (checkSection(pMode, pLineWords)) {
                continue;
            }

            //read sections
            if (*pMode == "files") {
                //this mode is in every example file, do we need it?
                if(verbose){
                    std::cout << "Line " << *pLineCount
                              << " - [WARNING] : the parser encountered the file mode, which is not supported at the moment.\n";
                }
            } else if (*pMode == "continents") {
                if (validateContinentLine(pContinentCount, pLineWords, pLineCount, pValidMap,verbose)) {
                    pContinentData->push_back(*pLineWords);
                } else {
                    return nullptr;
                }
            } else if (*pMode == "countries") {
                if (validateCountryLine(pCountryCount, pLineWords, pLineCount, pValidMap, pCountryID,
                                        pContinentCount,verbose)) {
                    pCountryData->push_back(*pLineWords);
                } else {
                    return nullptr;
                }
            } else if (*pMode == "borders") {
                auto* pLineNums = new std::vector<int>;
                if (validateBordersLine(pLineNums, pLineWords, pLineCount, pValidMap, pCountryCount,verbose)) {
                    pBorderData->push_back(*pLineNums);
                } else {
                    return nullptr;
                }
            } else if (pLineWords->at(0) == "name") {
                continue;
            } else {
                //unknown mode error, will be ignored, non-critical
                if(verbose){
                    std::cout << "\nLine " << *pLineCount << " - [WARNING] : the parser encountered an unknown mode. ";
                    std::cout << " ::  " << line;
                }
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

    if(!*pValidMap){
        return nullptr;
    }
    return map;
}

/**
 * Helper function that creates the map object
 * @param mapName, string of the map name
 * @param continentData, vector containing the name and id of each continent
 * @param countryData, vector containing the name and id of every country
 * @param borderData, vector containing adjacency information
 * @param vMap, flags map validity
 * @return Map
 */
Map* MapLoader::initMapObject(std::string* mapName, std::vector<std::vector<std::string>>* continentData,
                              std::vector<std::vector<std::string>>* countryData,
                              std::vector<std::vector<int>>* borderData, const bool* vMap) {
    if (*vMap) {
        //create map object with empty continents
        Map* gameMap = new Map(*mapName, *continentData);
        //add countries to map and continents
        for (auto& countries : *countryData) {
            gameMap->addNode(std::stoi(countries[0]), countries[1], std::stoi(countries[2]));
        }

        //add adjacency
        for (auto& borders : *borderData) {
            for (unsigned long j = 1; j < borders.size(); j++) {
                gameMap->addEdge(borders[0], borders[j]);
            }
        }
        return gameMap;
    }
    //there was an error, map was not created
    return nullptr;
}

/**
 * Sets the map file to load
 * @param newMapFile, string of the map name
 */
void MapLoader::setMapFile(std::string newMapFile) {
    pMapFile = new std::string(std::move(newMapFile));
}

/**
 * Helper function that splits a map file's line by spaces
 * @param line, string map file line
 * @param pLineWords, pointer to a vector that holds the split up line
 */
void MapLoader::splitLine(const std::string& line, std::vector<std::string>* pLineWords) {
    //split each line into vector of words (split by spaces)
    std::istringstream iss(line);
    for (std::string s; iss >> s;) {
        pLineWords->push_back(s);
    }
}

/**
 * Helper function that checks if the current line holds the map name
 * @param mapName, pointer that holds the map name
 * @param lineWords, pointer to a vector that holds the split up line
 */
void MapLoader::getMapName(std::string* mapName, std::vector<std::string>* lineWords) {
    //get map name
    if (lineWords->at(0) == "name" || lineWords->at(0) == "Name") {
        for (unsigned long i = 1; i < lineWords->size(); i++) {
            *mapName += (*lineWords)[i] + " ";
        }
    }
}

/**
 * Helper function that checks if the current line specifies the start of a new section
 * @param mode, pointer string that holds the current map mode
 * @param lineWords, pointer to a vector that holds the split up line
 */
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

/**
 * Helper function that checks the validity of a continent line
 */
bool MapLoader::validateContinentLine(int* continentCount, std::vector<std::string>* lineWords, const int* lineCount,
                                      bool* validMap,bool verbose) {
    (*continentCount)++;
    //check validity of the line in this mode
    if (lineWords->size() < 2) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [ERROR] : a line in the continents declaration had missing tokens, map could not be created.\n";
        }
        *validMap = false;
        return false;
    } else if (lineWords->size() > 2) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [WARNING] : a line in  the continents declaration had extra tokens.\n";
        }
        return true;
    } else {
        return true;
    }
}

/**
 * Helper function that checks the validity of a country line
 */
bool MapLoader::validateCountryLine(int* countryCount, std::vector<std::string>* lineWords, const int* lineCount,
                               bool* validMap,
                               int* countryID, const int* continentCount,bool verbose) {
    (*countryCount)++;
    //check validity of the line in this mode
    if (lineWords->size() < 3) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [ERROR] : a line in the countries declaration had missing tokens, map could not be created.\n";
        }
        *validMap = false;
        return false;
    } else {
        if (lineWords->size() > 3 && verbose)
            std::cout << "Line " << *lineCount
                      << " - [WARNING] : a line in  the countries declaration had extra tokens.\n";
        //check country id matches with order and that it references a valid continent
        if (std::stoi((*lineWords)[0]) == *countryID && std::stoi((*lineWords)[2]) <= *continentCount) {
            (*countryID)++;
            return true;
        } else {
            if(verbose){
                std::cout << "Line " << *lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
            }
            *validMap = false;
            return false;
        }
    }
}

/**
 * Helper function that checks the validity of a border line
 */
bool MapLoader::validateBordersLine(std::vector<int>* lineNums, std::vector<std::string>* lineWords, const int* lineCount,
                               bool* validMap, const int* countryCount, bool verbose) {
    //check validity of the line in this mode
    if (lineWords->size() < 2) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [ERROR] : a line in the borders declaration had missing tokens, map could not be created.\n";
        }
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
                if(verbose){
                    std::cout << "Line " << *lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
                }
                *validMap = false;
                return false;
            }
        }
        return true;
    }
}

/**
 * AlternativeLoader constructor
 * @param mapFile
 */
AlternativeLoader::AlternativeLoader(const std::string& mapFile) {
    pDominationMapFile = new std::string(mapFile);
}

/**
 * AlternativeLoader destructor
 */
AlternativeLoader::~AlternativeLoader() {
    delete pDominationMapFile;
}

/**
 * AlternativeLoader copy constructor
 * @param toCopy
 */
AlternativeLoader::AlternativeLoader(const AlternativeLoader &toCopy) {
    pDominationMapFile = new std::string();
    *pDominationMapFile = *toCopy.pDominationMapFile;
}

/**
 * assignment operator
 */
void AlternativeLoader::operator=(AlternativeLoader &rhs) {
    this->pDominationMapFile = rhs.pDominationMapFile;
}

/**
 * Sets the map file to load
 * @param newMapFile, string of the map name
 */
void AlternativeLoader::altSetMapFile(std::string newMapFile) {
    pDominationMapFile = &newMapFile;
}

/**
 * Main method of the map loader. Parses the file and creates the map object
 * @return Map
 */
Map *AlternativeLoader::altReadMapFile() {
    //for debug purposes, change to true to print errors and warnings
    bool verbose = false;
    //create file stream to read file line by line
    std::ifstream infile(*pDominationMapFile);

    if(!infile || infile.peek() == EOF){
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
            // strip every line of the \n and \r characters
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            //check if we enter a new section
            if (altCheckSection(pMode, line)) {
                continue;
            }
            altSplitLine(line, pLineWords, *pMode);
            altGetMapName(pMapName, pLineWords);

            //read sections
            if (*pMode == "Map") {
                //this mode is in every example file, do we need it?
                if(verbose){
                    std::cout << "Line " << *pLineCount
                              << " - [WARNING] : the parser encountered the Map mode, which is not supported at the moment.\n";
                }
            } else if (*pMode == "Continents") {
                if (altValidateContinentLine(pContinentCount, pLineWords, pLineCount, pValidMap,verbose)) {
                    pContinentData->push_back(*pLineWords);
                } else {
                    infile.close();
                    return nullptr;
                }
            } else if (*pMode == "Territories") {
                if (altValidateTerritoriesLine(pLineWords, pLineCount, pValidMap,verbose)) {
                    pTerritoryData->push_back(*pLineWords);
                } else {
                    infile.close();
                    return nullptr;
                }
            } else {
                //unknown mode error, will be ignored, non-critical
                if(verbose){
                    std::cout << "\nLine " << *pLineCount << " - [WARNING] : the parser encountered an unknown mode. ";
                    std::cout << " ::  " << line;
                }
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

    infile.close();
    return map;
}

/**
 * Helper function that checks if the current line specifies the start of a new section
 * @param mode, pointer string that holds the current map mode
 * @param line, current line
 */
bool AlternativeLoader::altCheckSection(std::string *mode, const std::string &line) {
    //separate in sections
    if (line[0] == *"[") {
        *mode = "";
        for (unsigned long i = 1; i < line.size() - 1; i++) {
            if(line[i] != *"]") {
                *mode += line[i];
            }
        }
        return true;
    }
    return false;
}

/**
 * Helper function that creates the map object
 * @param mapName, string of the map name
 * @param continentData, vector containing the name and id of each continent
 * @param territoryData, vector containing the country and adjacency information
 * @param vMap, flags map validity
 * @return Map
 */
Map *AlternativeLoader::altInitMapObject(std::string *mapName, std::vector<std::vector<std::string>> *continentData,
                                         std::vector<std::vector<std::string>> *territoryData, const bool *vMap) {
    if (*vMap) {
        //create map object with empty continents
        Map* gameMap = new Map(*mapName, *continentData);

        //format data into 3 vectors that map expects
        std::vector<std::vector<std::string>> countryData;
        countryData.resize(territoryData->size());
        countryData = altGetCountryData(countryData,territoryData,continentData);
        std::vector<std::string> countryList;
        countryList = altGetCountryList(countryList,territoryData);
        std::vector<std::vector<int>> borderData;
        borderData.resize(territoryData->size());
        borderData = altGetBorderData(borderData,territoryData,countryList);
        //bail if there was an error while formatting the data
        if(borderData.empty() || countryData.empty()){
            return nullptr;
        }
        //add countries to map
        for (auto& countries : countryData) {
            gameMap->addNode(std::stoi(countries[0]), countries[1], std::stoi(countries[2]));
        }
        //add adjacency
        for (auto& borders : borderData) {
            for (unsigned long j = 1; j < borders.size(); j++) {
                gameMap->addEdge(borders[0], borders[j]);
            }
        }
        return gameMap;
    }
    //there was an error, map was not created
    return nullptr;
}

/**
 * Helper function that splits a map file's line by spaces
 * @param line, string map file line
 * @param pLineWords, pointer to a vector that holds the split up line
 * @param mode, current reading mode
 */
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
        pLineWords->push_back(mLine);
    }else{
        //split each line into vector of words (split by ,)
        delimiter = ",";
        while((pos = mLine.find(delimiter)) != std::string::npos){
            token = mLine.substr(0,pos);
            pLineWords->push_back(token);
            mLine.erase(0,pos + delimiter.length());
        }
        pLineWords->push_back(mLine);
    }
}

/**
 * Helper function that checks if the current line holds the map name
 * @param mapName, pointer that holds the map name
 * @param lineWords, pointer to a vector that holds the split up line
 */
void AlternativeLoader::altGetMapName(std::string *mapName, std::vector<std::string> *lineWords) {
    //get map name
    if (lineWords->at(0) == "name" || lineWords->at(0) == "Name") {
        for (unsigned long i = 1; i < lineWords->size(); i++) {
            *mapName += (*lineWords)[i] + " ";
        }
    }
}

/**
 * Helper function that checks the validity of a continent line
 */
bool AlternativeLoader::altValidateContinentLine(int *continentCount, std::vector<std::string> *lineWords, const int *lineCount,
                                            bool *validMap,bool verbose) {
    (*continentCount)++;
    //check validity of the line in this mode
    if (lineWords->size() < 2) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [ERROR] : a line in the continents declaration had missing tokens, map could not be created.\n";
        }
        *validMap = false;
        return false;
    } else if (lineWords->size() > 2) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [WARNING] : a line in  the continents declaration had extra tokens.\n";
        }
        return true;
    } else {
        return true;
    }
}

/**
 * Helper function that checks the validity of a territory line
 */
bool AlternativeLoader::altValidateTerritoriesLine(std::vector<std::string> *lineWords, const int *lineCount,
                                              bool *validMap,bool verbose) {
    //check validity of the line in this mode
    if (lineWords->size() < 5) {
        if(verbose){
            std::cout << "Line " << *lineCount
                      << " - [ERROR] : a line in the borders declaration had missing tokens, map could not be created.\n";
        }
        *validMap = false;
        return false;
    }
    return true;
}

/**
 * Helper function that goes through the list of continents and returns the id
 */
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

/**
 * Helper function that goes through the list of countries and returns the id
 */
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

/**
 * Helper function that looks through the territory data and extracts the country data
 */
std::vector<std::vector<std::string>> AlternativeLoader::altGetCountryData(std::vector<std::vector<std::string>> countries,
                                                                           std::vector<std::vector<std::string>> *territories,
                                                                           std::vector<std::vector<std::string>> *continents) {
    std::vector<std::vector<std::string>> to_return = std::move(countries);
    unsigned int counter = 0;
    //populate countryData
    for(auto& i : *territories){
        to_return[counter].push_back(std::to_string(counter + 1));
        to_return[counter].push_back(i[0]);
        if(getContinentID(i[3],*continents) == "0"){
            //there was an error, map was not created
            return std::vector<std::vector<std::string>>();
        }
        to_return[counter].push_back(getContinentID(i[3],*continents));
        counter++;
    }
    return to_return;
}

/**
 * Helper function that creates the country list
 */
std::vector<std::string> AlternativeLoader::altGetCountryList(std::vector<std::string> countries,
                                                              std::vector<std::vector<std::string>> *territories) {
    std::vector<std::string> to_return = std::move(countries);
    //populate countrylist
    for(auto& i : *territories){
        to_return.push_back(i[0]);
    }
    return to_return;
}

/**
 * Helper function that looks through the territory data and extracts the border data
 */
std::vector<std::vector<int>> AlternativeLoader::altGetBorderData(std::vector<std::vector<int>> borders,
                                                                  std::vector<std::vector<std::string>> *territories,
                                                                  const std::vector<std::string>& countries) {
    //populate border data
    std::vector<std::vector<int>> to_return = std::move(borders);
    int counter = 0;
    for(auto& i : *territories){
        to_return[counter].push_back(counter + 1);
        for(unsigned int j = 4; j < i.size(); j++){
            if(getCountryID(i[j],countries) == 0){
                //there was an error, map was not created
                return std::vector<std::vector<int>>();
            }
            to_return[counter].push_back(getCountryID(i[j],countries));
        }
        counter++;
    }
    return to_return;
}
