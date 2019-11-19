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
    class Continent;
    class Country;

    Map(std::string mapTitle,std::vector<std::vector<std::string>> ctd);

    Map(std::string& mapTitle, std::vector<Continent*> continents);
    ~Map();
    Map(const Map& toCopy);
    void operator=(Map& rhs);
    void printMap();
    bool testConnected();

    inline std::string* getMapTitle() const { return pMapTitle; };
    class Country{
        public:
            Country(int id, std::string name, int continent);
            void operator=(Country& rhs);
            ~Country();
            inline std::vector<Country*>* getAdjCountries(){return pAdjCountries;};
            inline std::string getCountryName() { return *cyName; }
            inline void setPlayerOwnerID(int id) { *pPlayerOwnerId = id; }
            inline void setNumberOfTroops(int troops) { *pNumberOfTroops = troops; };
            inline int getPlayerOwnerID(){ return *pPlayerOwnerId;};
            inline int getNumberOfTroops(){ return *pNumberOfTroops;};
            inline int getCountryContinent(){return *cyContinent;};
            inline int getCountryId() {return *cyID; };
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
        Continent(Map::Continent& toCopy);
        void operator=(Continent& rhs);
        inline std::vector<Country*>* getCountriesInContinent(){return pCountriesInContinent;};
        inline std::string getContinentName() { return *pCName; }
        inline int getpCTroops() { return *pCTroops; }
        inline void addCountry(Country* country) { pCountriesInContinent->push_back(country); }

    private:
        std::vector<Country*>* pCountriesInContinent;
        std::string* pCName;
        int* pCTroops;
    };

    Map::Country* addNode(int id, std::string name, int continent);
    void addEdge(int from, int to);
    inline std::vector<Country*>* getMapCountries(){return pMapCountries;};

    inline void setMapCountries(std::vector<Country*>* c) { pMapCountries = c; }
    inline std::vector<Continent*>* getMapContinents(){return pMapContinents;};

private:
    std::string* pMapTitle;
    std::vector<Continent*>* pMapContinents;
    std::vector<Country*>* pMapCountries;
    static bool testIndividualGraph(std::vector<Country*>* toTest, bool isContinent);
    static void dfs(std::set<std::string>* visitedCountries,Country* countries, bool continentTest);
};

#endif //COMP_345_PROJ_MAP_H