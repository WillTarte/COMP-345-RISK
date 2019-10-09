//
// Created by lebou on 2019-09-15.
//

#include <iostream>
#include "../../include/Map.h"

int main()
{
    //more extensive tests are performed in the MapLoader (on the data being submitted)

    //declare valid map information
    std::string validMapName = "vTest map";
    std::vector<std::vector<std::string>> validContinentData = {{"europe","2"},{"asia","4"}};
    std::vector<std::vector<std::string>> validCountryData = {{"1","france","1"},{"2","germany","1"},{"3","russia","2"}};
    std::vector<std::vector<int>> validBorderData = {{1,2},{2,1,3},{3,2}};
    //create map object with empty continents
    Map validMap = Map(validMapName,validContinentData);
    //add countries to map and continents
    for(auto & i : validCountryData){
        validMap.addNode(std::stoi(i[0]),i[1],std::stoi(i[2]));
    }

    //add adjacency
    for(auto & i : validBorderData){
        for(unsigned long j=1; j<i.size(); j++) {
            validMap.addEdge(i[0],i[j]);
        }
    }

    //declare invalid map information
    std::string invalidMapName = "vTest map";
    std::vector<std::vector<std::string>> invalidContinentData = {{"europe","2"},{"asia","4"}};
    std::vector<std::vector<std::string>> invalidCountryData = {{"1","france","1"},{"2","germany","1"},{"3","russia","2"}};
    std::vector<std::vector<int>> invalidBorderData = {{1,2},{2,1},{3}};
    //create map object with empty continents
    Map invalidMap = Map(invalidMapName,invalidContinentData);
    //add countries to map and continents
    for(auto & i : validCountryData){
        invalidMap.addNode(std::stoi(i[0]),i[1],std::stoi(i[2]));
    }

    //add adjacency
    for(auto & i : invalidBorderData){
        for(unsigned long j=1; j<i.size(); j++) {
            invalidMap.addEdge(i[0],i[j]);
        }
    }

    //test maps
    validMap.printMap();
    invalidMap.printMap();

    return 0;
}