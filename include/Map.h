//
// Created by lebou on 2019-09-08.
//

#include <string>
#include <vector>

#ifndef COMP_345_PROJ_MAP_H
#define COMP_345_PROJ_MAP_H

class Map{
public:
    Map(std::string mapTitle,std::vector<std::vector<std::string>> ctd);
    std::string* pMapTitle;

    class Country{
    public:
        Country(int id, std::string name, int continent);
        std::vector<Country*>* pAdjCountries;
        std::string getCountry(){ return *cyName; }
    private:
        int* cyID;
        std::string* cyName;
        int* cyContinent;
        //these properties will be populated later
        int* pPlayerOwnerId;
        int* pNumberOfTroops;
    };
    class Continent{
    public:
        Continent(std::string cname, int troops);
        std::vector<Country*>* pCountriesInContinent;
        void setCountry(Map::Country* c);
        std::string getContinent(){ return *pCName; }
    private:
        std::string* pCName;
        int* pCTroops;
    };
    std::vector<Continent*>* pMapContinents;
    std::vector<Country*>* pMapCountries;
    Map::Country* addNode(int id, std::string name, int continent);
    void addEdge(int from, int to);
    void printMap();
    bool testConnected();
};

#endif //COMP_345_PROJ_MAP_H