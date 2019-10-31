//
// Created by lebou on 2019-09-08.
//

#include <string>
#include <vector>
#include <set>

#ifndef COMP_345_PROJ_MAP_H
#define COMP_345_PROJ_MAP_H

class Map {
public:
    Map(std::string mapTitle,std::vector<std::vector<std::string>> ctd);
    std::string* pMapTitle;
    void printMap();
    bool testConnected();
    class Country{
    public:
        Country(int id, std::string name, int continent);
        std::vector<Country*>* pAdjCountries;
        std::string getCountryName() { return *cyName; }
        void setPlayerOwnerID(int id) { *pPlayerOwnerId = id; }
        void setNumberOfTroops(int troops) { *pNumberOfTroops = troops; };
        int getPlayerOwnerID();
        int getNumberOfTroops();

    private:
        int* cyID;
        std::string* cyName;
        int* cyContinent;
        //these properties will be populated later
        int* pPlayerOwnerId;
        int* pNumberOfTroops;
    };

    class Continent {
    public:
        Continent(std::string cname, int troops);
        std::vector<Country*>* pCountriesInContinent;
        void setCountry(Map::Country* c);
        std::string getContinentName() { return *pCName; }

    private:
        std::string* pCName;
        int* pCTroops;
    };
    Map::Country* addNode(int id, std::string name, int continent);
    void addEdge(int from, int to);
    std::vector<Country*>* getMapCountries(){return pMapCountries;};
private:
    std::vector<Continent*>* pMapContinents;
    std::vector<Country*>* pMapCountries;
    void dfs(std::set<std::string>* visitedCountries,Country* countries);
};

bool checkIfAdjacent(Map::Country& country1, Map::Country& country2);

#endif //COMP_345_PROJ_MAP_H