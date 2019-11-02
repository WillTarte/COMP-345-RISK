//
// Created by lebou on 2019-09-08.
//

#include "../include/Map.h"
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <iostream>

/**
 * Map constructor
 *
 * @param mapTitle the name of the map
 * @param ctd the continents data
 */
Map::Map(std::string mapTitle, std::vector<std::vector<std::string>> ctd) {
    pMapTitle = new std::string(std::move(mapTitle));
    pMapContinents = new std::vector<Continent*>;
    pMapCountries = new std::vector<Country*>;
    //create empty continents
    for (auto& i : ctd) {
        //The map class stores a vector of pointers to Continents, this is essentially a vector containing all the subgraphs of the map
        pMapContinents->push_back(new Continent(i[0], std::stoi(i[1])));
    }
}

/**
 * Continent constructor, subclass of Map
 *
 * @param cname the continent name
 * @param troops the number of troops
 */
Map::Continent::Continent(std::string cname, int troops) {
    pCName = new std::string(std::move(cname));
    pCTroops = new int(troops);
    pCountriesInContinent = new std::vector<Country*>;
}

/**
 * add a country to this continent, it does not create a copy of the country, it only adds its pointer to a list
 * of pointers. It is used to keep track of which nodes compose each subgraph.
 *
 * @param c the country to add
 */
void Map::Continent::setCountry(Map::Country* c) {
    pCountriesInContinent->push_back(c);
}

/**
 * Country constructor, subclass of Map
 *
 * @param id the country id
 * @param name the country name
 * @param continent the continent id
 */
Map::Country::Country(int id, std::string name, int continent) {
    cyID = new int(id);
    cyName = new std::string(std::move(name));
    // each country only has one continent (-1 because we transform continent count to continent index)
    cyContinent = new int(continent - 1);
    pAdjCountries = new std::vector<Country*>;
    //I dont know what to do with those
    pPlayerOwnerId = new int(0);
    pNumberOfTroops = new int(0);
}

/**
 * Add connection between two countries
 *
 * @param id the country id
 * @param name the country name
 * @param continent the continent id
 */
Map::Country* Map::addNode(int id, std::string name, int continent) {
    //create graph node
    auto* thisCountry = new Country(id, std::move(name), continent);
    //add node to graph (overall graph)
    pMapCountries->push_back(thisCountry);
    //add node to relevant subgraph
    (*pMapContinents)[continent - 1]->setCountry(thisCountry);
    return thisCountry;
}

/**
 * Add connection between two countries
 *
 * @param from the first country id
 * @param to the second country id
 */
void Map::addEdge(int from, int to) {
    //each node maintains a list of pointers to all neighboor nodes
    (*pMapCountries)[from - 1]->getAdjCountries()->push_back((*pMapCountries)[to - 1]);
}

/**
 * Test if the map is a connected graph.
 *
 * @return true if the map is a connected graph
 */
bool Map::testConnected() {
    //test overall graph
    if(testIndividualGraph(this->pMapCountries,false)){
        //test subgraphs
        for(auto continent : *this->pMapContinents){
            if(continent->getCountriesInContinent()->size() == 1){
                continue;
            }else if(!testIndividualGraph(continent->getCountriesInContinent(),true)){
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Map::testIndividualGraph(std::vector<Country*>* toTest, bool isContinent){
    auto* visitedCountries = new std::set<std::string>;
    //recurse through graph with an arbitrary starting point, if we visit all the countries in graph then its connected.
    dfs(visitedCountries,toTest[0][0],isContinent);
    bool connected = visitedCountries->size() == toTest->size();
    delete (visitedCountries);
    return connected;
}

/**
 * Test if the map is a connected graph.
 * recurse through all adjacent countries from starting point and add country name to list. If all the countries are in the list after the recursion, then map is fully connected
 *
 * @param visitedCountries a list of visited country names
 * @param country the country we are analyzing
 */
void Map::dfs(std::set<std::string>* visitedCountries,Country* country, bool continentTest){
    if(visitedCountries->find(country->getCountryName()) == visitedCountries->end()){
        visitedCountries->insert(country->getCountryName());
        for(auto& c : *country->getAdjCountries()) {
            if(!continentTest || c->getCountryContinent() == country->getCountryContinent()){
                dfs(visitedCountries,c, continentTest);
            }else{
                continue;
            }
        }
    }
}

/**
 * Print and test all relevant map information
 *
 */
void Map::printMap() {
    for (auto& pMapContinent : *this->pMapContinents) {
        std::string currContinent = pMapContinent->getContinentName();
        std::vector<Map::Country*> countriesInCurrContinent = *pMapContinent->getCountriesInContinent();
        std::cout << " \n\n\n ######################## \n COUNTRIES IN CONTINENT : " << currContinent
                  << "\n ######################## \n";
        for (auto& j : countriesInCurrContinent) {
            std::string currCountryName = j->getCountryName();
            std::cout << currCountryName << " is owned by player " << j->getPlayerOwnerID() <<" and has " << j->getNumberOfTroops() << " troops \n";
        }
    }
}

/**
 * Checks if two countries are adjacent, by country name
 *
 * @param country1 the first country
 * @param country2 the second country
 * @return true if both are adjacent to each other
 */
bool checkIfAdjacent(Map::Country& country1, Map::Country& country2) {

    bool oneAdjacentTwo = false;
    bool twoAdjacentOne = false;

    for (auto& i : *country1.getAdjCountries()) {
        if (i->getCountryName() == country2.getCountryName()) {
            oneAdjacentTwo = true;
        }
    }

    for (auto& i : *country2.getAdjCountries()) {
        if (i->getCountryName() == country1.getCountryName()) {
            twoAdjacentOne = true;
        }
    }
    return oneAdjacentTwo && twoAdjacentOne;
}