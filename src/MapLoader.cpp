//
// Created by lebou on 2019-09-15.
//
#include "../include/Map.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "../include/MapLoader.h"

MapLoader::MapLoader(std::string mapFile) {
    pMapFile = &mapFile;
}

Map* MapLoader::initMap() {
    //create file stream to read file line by line
    std::ifstream infile(*pMapFile);
    std::string line;

    //declare map info
    std::string mapName;
    std::vector<std::vector<std::string>> continentData;
    std::vector<std::vector<std::string>> countryData;
    std::vector<std::vector<int>> borderData;

    std::string mode = "";

    bool validMap = true;

    //maintain a counter of the file lines, for error tracing
    int lineCount = 0;
    //maintain a counter to validate country id
    int countryID = 1;
    //maintain a continent counter to validate country insertion
    int continentCount = 0;
    //maintain a country counter to validate edge insertion
    int countryCount = 0;

    while (std::getline(infile, line)){
        //ignore comments or empty lines
        if(line.empty() || line[0] == *";"){
            //do nothing, the line is empty or is a comment
        }else{
            //split each line into vector of words (split by spaces)
            std::vector<std::string> lineWords;
            std::istringstream iss(line);
            for(std::string s; iss >> s; )
                lineWords.push_back(s);

            //get map name
            if(lineWords[0] == "name" || lineWords[0] == "Name"){
                for(int i = 1; i<lineWords.size(); i++) {
                    mapName += lineWords[i] + " ";
                }
            }

            //separate in sections
            if(lineWords[0][0] == *"["){
               mode = "";
               for(int i = 1; i<lineWords[0].size()-1; i++){
                   mode += lineWords[0][i];
               }
               continue;
            }

            //read sections
            if(mode == "files"){
                //this mode is in every example file, do we need it?
                std::cout << "Line " << lineCount << " - [WARNING] : the parser encountered the file mode, which is not supported at the moment.\n";
            }else if(mode == "continents") {
                continentCount ++;
                //check validity of the line in this mode
                if (lineWords.size() < 2) {
                    std::cout << "Line " << lineCount << " - [ERROR] : a line in the continents declaration had missing tokens, map could not be created.\n";
                    validMap = false;
                }else if(lineWords.size() > 2){
                    std::cout << "Line " << lineCount << " - [WARNING] : a line in  the continents declaration had extra tokens.\n";
                    continentData.push_back(lineWords);
                }else{
                    continentData.push_back(lineWords);
                }
            }else if(mode == "countries"){
                countryCount ++;
                //check validity of the line in this mode
                if (lineWords.size() < 3) {
                    std::cout << "Line " << lineCount << " - [ERROR] : a line in the countries declaration had missing tokens, map could not be created.\n";
                    validMap = false;
                }else if(lineWords.size() > 3){
                    std::cout << "Line " << lineCount << " - [WARNING] : a line in  the countries declaration had extra tokens.\n";
                    //check country id matches with order and that it references a valid continent
                    if(std::stoi(lineWords[0]) == countryID && std::stoi(lineWords[2]) <= continentCount){
                        countryData.push_back(lineWords);
                        countryID ++;
                    }else{
                        std::cout << "Line " << lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
                        validMap = false;
                    }
                }else{
                    //check country id matches with order and that it references a valid continent
                    if(std::stoi(lineWords[0]) == countryID && std::stoi(lineWords[2]) <= continentCount){
                        countryData.push_back(lineWords);
                        countryID ++;
                    }else{
                        std::cout << "Line " << lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
                        validMap = false;
                    }
                }
            }else if(mode == "borders"){
                //check validity of the line in this mode
                if(lineWords.size() < 2){
                    std::cout << "Line " << lineCount << " - [ERROR] : a line in the borders declaration had missing tokens, map could not be created.\n";
                    validMap = false;
                }else{
                    //convert strings to ints
                    std::vector<int> lineNums;
                    for (auto &s : lineWords) {
                        std::stringstream parser(s);
                        int x = 0;
                        parser >> x;
                        lineNums.push_back(x);
                        //check that all countries referenced in this line exist
                        if(x > countryCount){
                            std::cout << "Line " << lineCount << " - [ERROR] : a country or continent ID did was invalid.\n";
                            validMap = false;
                        }
                    }
                    borderData.push_back(lineNums);
                }
            }else{
                //unknown mode error, will be ignored, non-critical
                std::cout << "Line " << lineCount << " - [WARNING] : the parser encountered an unknown mode.\n";
            }
        }
        lineCount++;
    }
    //create map object
    return initMapObject(mapName,continentData,countryData,borderData,validMap);
}

Map* MapLoader::initMapObject(std::string mapName, std::vector<std::vector<std::string>> continentData, std::vector<std::vector<std::string>> countryData, std::vector<std::vector<int>> borderData, BOOLEAN vMap){
    if(vMap){
        //create map object with empty continents
        Map* gameMap = new Map(mapName,continentData);
        //add countries to map and continents
        for(auto & i : countryData){
            gameMap->addNode(std::stoi(i[0]),i[1],std::stoi(i[2]));
        }

        //add adjacency
        for(auto & i : borderData){
            for(int j=1; j<i.size(); j++) {
                gameMap->addEdge(i[0],i[j]);
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