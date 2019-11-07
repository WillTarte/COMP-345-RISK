//
// Created by lebou on 2019-09-08.
//

#include "../include/Map.h"
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <iostream>
using std::string;
using std::vector;

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
 * Map copy constructor
 * @param toCopy
 */
Map::Map(const Map& toCopy) {
    pMapTitle = new string();
    pMapContinents = new vector<Continent*>;
    pMapCountries = new vector<Country*>;
    *pMapTitle = *toCopy.pMapTitle;
    *pMapContinents = *toCopy.pMapContinents;
    *pMapCountries = *toCopy.pMapCountries;
}

/**
 * Map destructor
 */
Map::~Map() {
    delete pMapTitle;
    delete pMapContinents;
    delete pMapCountries;
}

/**
 * assignment operator
 * @param rhs
 */
void::Map::operator=(Map& rhs) {
    this->pMapTitle = rhs.pMapTitle;
    this->pMapContinents = rhs.pMapContinents;
    this->pMapCountries = rhs.pMapCountries;
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
 * Continent destructor
 */
Map::Continent::~Continent() {
    delete pCName;
    delete pCTroops;
    delete pCountriesInContinent;
}

/**
 * Continent copy constructor
 * @param toCopy
 */
Map::Continent::Continent(Map::Continent &toCopy) {
    pCName = new string();
    pCTroops = new int();
    pCountriesInContinent = new vector<Country*>;
    *pCName = *toCopy.pCName;
    *pCTroops = *toCopy.pCTroops;
    *pCountriesInContinent = *toCopy.pCountriesInContinent;
}

/**
 * assignment operator
 * @param rhs
 */
void Map::Continent::operator=(Map::Continent& rhs) {
    this->pCName = rhs.pCName;
    this->pCTroops = rhs.pCTroops;
    this->pCountriesInContinent = rhs.pCountriesInContinent;
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
    pPlayerOwnerId = new int(-1);
    pNumberOfTroops = new int(0);
}

/**
 * Country destructor
 */
Map::Country::~Country() {
    delete cyID;
    delete cyName;
    delete cyContinent;
    delete pAdjCountries;
    delete pPlayerOwnerId;
    delete pNumberOfTroops;
}

void Map::Country::operator=(Map::Country& rhs){
    this->cyID = rhs.cyID;
    this->cyName = rhs.cyName;
    this->cyContinent = rhs.cyContinent;
    this->pPlayerOwnerId = rhs.pPlayerOwnerId;
    this->pNumberOfTroops = rhs.pNumberOfTroops;
    this->pAdjCountries = rhs.pAdjCountries;
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
    (*pMapContinents)[continent - 1]->addCountry(thisCountry);
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

Map::Map(std::string& mapTitle, vector<Continent*> continents) {
    pMapTitle = &mapTitle;
    pMapContinents = &continents;
}