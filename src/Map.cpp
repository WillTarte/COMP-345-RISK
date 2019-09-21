//
// Created by lebou on 2019-09-08.
//

#include "../include/Map.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <memory>
#include <winnt.h>

//Map constructor
Map::Map(std::string mapTitle, std::vector<std::vector<std::string>> ctd) {
    pMapTitle = new std::string(mapTitle);
    pMapContinents = new std::vector<Continent*>;
    pMapCountries = new std::vector<Country*>;
    //create empty continents
    for(auto & i : ctd){
        //The map class stores a vector of pointers to Continents, this is essentially a vector containing all the subgraphs of the map
        pMapContinents->push_back(new Continent(i[0], std::stoi(i[1])));
    }
}

//Continent constructor, subclass of Map
Map::Continent::Continent(std::string cname, int troops) {
    pCName = new std::string(cname);
    pCTroops = new int(troops);
    pCountriesInContinent = new std::vector<Country*>;
}

/*
add a country to this continent, it does not create a copy of the country, it only adds its pointer to a list
of pointers. It is used to keep track of which nodes compose each subgraph.
 */
void Map::Continent::setCountry(Map::Country *c) {
    pCountriesInContinent->push_back(c);
}

//Country constructor, subclass of Map
Map::Country::Country(int id, std::string name, int continent) {
    cyID = new int(id);
    cyName = new std::string(name);
    // each country only has one continent (-1 because we transform continent count to continent index)
    cyContinent = new int(continent-1);
    pAdjCountries = new std::vector<Country*>;
    //I dont know what to do with those
    pPlayerOwnerId = new int(0);
    pNumberOfTroops = new int(0);
}

int Map::Country::getNumberOfTroops() {
    return *pNumberOfTroops;
}

int Map::Country::getPlayerOwnerID() {
    return *pPlayerOwnerId;
}

Map::Country* Map::addNode(int id, std::string name, int continent) {
    //create graph node
    auto* thisCountry = new Country(id,name,continent);
    //add node to graph (overall graph)
    pMapCountries->push_back(thisCountry);
    //add node to relevant subgraph
    (*pMapContinents)[continent-1]->setCountry(thisCountry);
    return thisCountry;
}

void Map::addEdge(int from, int to) {
    //each node maintains a list of pointers to all neighboor nodes
    (*pMapCountries)[from-1]->pAdjCountries->push_back((*pMapCountries)[to-1]);
}

//test if the map is a connected graph, if it is, its subgraphs are also connected
bool Map::testConnected() {
    for(auto & countries : *this->pMapCountries){
        if(countries->pAdjCountries->empty()){
            return false;
        }
    }
    return true;
}

//helper function to print and test all relevant map information
void Map::printMap() {
    for(auto & pMapContinent : *this->pMapContinents){
        std::string currContinent = pMapContinent->getContinentName();
        std::vector<Map::Country*> countriesInCurrContinent = *pMapContinent->pCountriesInContinent;
        std::cout << " \n\n\n ######################## \n COUNTRIES IN CONTINENT : " << currContinent << "\n ######################## \n";
        for(auto & j : countriesInCurrContinent){
            std::string currCountryName = j->getCountryName();
            std::cout << currCountryName << " is adjacent to :\n";
            for(auto & pAdjCountry : *j->pAdjCountries){
                std::string currAdjCountry = pAdjCountry->getCountryName();
                std::cout << "     -" << currAdjCountry << "\n";
            }
        }
    }

    std::cout << "\n\n $$$$$$$$$$$$$$$$$$$$$$$$ \n TESTING MAP VALIDITY \n $$$$$$$$$$$$$$$$$$$$$$$$ \n\n";
    if(testConnected()){
        std::cout << "MAP IS VALID";
    }else{
        std::cout << "MAP IS INVALID";
    }

}





