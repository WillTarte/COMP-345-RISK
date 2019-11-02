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
    ~Map();
    void printMap();
    bool testConnected();
    std::string* getMapTitle(){return pMapTitle;};
    class Country{
    public:
        Country(int id, std::string name, int continent);
        ~Country();
        std::vector<Country*>* getAdjCountries(){return pAdjCountries;};
        std::string getCountryName() { return *cyName; }
        void setPlayerOwnerID(int id) { *pPlayerOwnerId = id; }
        void setNumberOfTroops(int troops) { *pNumberOfTroops = troops; };
        int getPlayerOwnerID(){ return *pPlayerOwnerId;};
        int getNumberOfTroops(){ return *pNumberOfTroops;};
        int getCountryContinent(){return *cyContinent;};
    private:
        int* cyID;
        std::string* cyName;
        int* cyContinent;
        int* pPlayerOwnerId;
        int* pNumberOfTroops;
        std::vector<Country*>* pAdjCountries;
    };

    class Continent {
    public:
        Continent(std::string cname, int troops);
        ~Continent();
        std::vector<Country*>* getCountriesInContinent(){return pCountriesInContinent;};
        void setCountry(Map::Country* c);
        std::string getContinentName() { return *pCName; }

    private:
        std::vector<Country*>* pCountriesInContinent;
        std::string* pCName;
        int* pCTroops;
    };
    Map::Country* addNode(int id, std::string name, int continent);
    void addEdge(int from, int to);
    std::vector<Country*>* getMapCountries(){return pMapCountries;};
private:
    std::string* pMapTitle;
    std::vector<Continent*>* pMapContinents;
    std::vector<Country*>* pMapCountries;
    static bool testIndividualGraph(std::vector<Country*>* toTest, bool isContinent);
    static void dfs(std::set<std::string>* visitedCountries,Country* countries, bool continentTest);
};

bool checkIfAdjacent(Map::Country& country1, Map::Country& country2);

#endif //COMP_345_PROJ_MAP_H