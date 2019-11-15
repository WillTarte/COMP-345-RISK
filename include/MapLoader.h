//
// Created by lebou on 2019-09-15.
//
#include <vector>
#include <string>
#include "../include/Map.h"

#ifndef COMP_345_PROJ_MAPLOADER_H
#define COMP_345_PROJ_MAPLOADER_H

class string;

class MapLoader {
public:
    explicit MapLoader(std::string mapFile = "");
    ~MapLoader();
    MapLoader(const MapLoader& toCopy);
    void operator=(MapLoader& rhs);
    std::string* pMapFile;
    void setMapFile(std::string newMapFile);
    virtual Map* readMapFile();

private:
    static Map* initMapObject(std::string* mapName, std::vector<std::vector<std::string>>* continentData,
                              std::vector<std::vector<std::string>>* countryData,
                              std::vector<std::vector<int>>* borderData, const bool* vMap);
    void splitLine(const std::string& line, std::vector<std::string>* pLineWords);
    void getMapName(std::string* mapName, std::vector<std::string>* lineWords);
    bool checkSection(std::string* mode, std::vector<std::string>* lineWords);
    bool validateContinentLine(int* continentCount, std::vector<std::string>* lineWords, const int* lineCount,
                               bool* validMap);
    bool validateCountryLine(int* countryCount, std::vector<std::string>* lineWords, const int* lineCount, bool* validMap,
                        int* countryID, const int* continentCount);
    bool validateBordersLine(std::vector<int>* lineNums, std::vector<std::string>* lineWords, const int* lineCount,
                             bool* validMap, const int* countryCount);
};

class AlternativeLoader{
public:
    AlternativeLoader(std::string mapFile = "");
    ~AlternativeLoader();
    AlternativeLoader(const AlternativeLoader& toCopy);
    void operator=(AlternativeLoader& rhs);
    std::string* pDominationMapFile;
    void altSetMapFile(std::string newMapFile);
    virtual Map* altReadMapFile();
private:
    static Map* altInitMapObject(std::string* mapName, std::vector<std::vector<std::string>>* continentData,
                                 std::vector<std::vector<std::string>>* territoryData, const bool* vMap);
    void altSplitLine(const std::string &line, std::vector<std::string> *pLineWords, const std::string& mode);
    void altGetMapName(std::string *mapName, std::vector<std::string> *lineWords);
    bool altCheckSection(std::string* mode, const std::string &line);
    bool altValidateContinentLine(int* continentCount, std::vector<std::string>* lineWords, const int* lineCount,
                                  bool* validMap);
    bool altValidateTerritoriesLine(std::vector<std::string> *lineWords, const int *lineCount,
                                    bool *validMap);
    static std::string getContinentID(const std::string& continentName,std::vector<std::vector<std::string>> continentData);
    static int getCountryID(const std::string& countryName, const std::vector<std::string>& countryList);

    static std::vector<std::vector<std::string>>
    altGetCountryData(std::vector<std::vector<std::string>> countries, std::vector<std::vector<std::string>> *territories,
                      std::vector<std::vector<std::string>> *continents);

    static std::vector<std::string>
    altGetCountryList(std::vector<std::string> countries, std::vector<std::vector<std::string>> *territories);

    static std::vector<std::vector<int>>
    altGetBorderData(std::vector<std::vector<int>> borders, std::vector<std::vector<std::string>> *territories,
                     const std::vector<std::string>& countries);
};

class MapLoaderAdapter : public MapLoader{
private:
    AlternativeLoader AltLoader;
public:
    MapLoaderAdapter(AlternativeLoader alt){AltLoader = alt;};
    Map* readMapFile(){
        return AltLoader.altReadMapFile();
    };
};

#endif //COMP_345_PROJ_MAPLOADER_H


