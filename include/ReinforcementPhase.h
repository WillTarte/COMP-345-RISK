#include "Map.h"
#include "Player.h"
#include <vector>

#ifndef COMP_345_PROJ_REINFORCEMENTPHASE_H
#define COMP_345_PROJ_REINFORCEMENTPHASE_H

class ReinforcementPhase {
public:
    ReinforcementPhase(Player player,
            std::vector<Map::Country> countries,
            std::vector<Map::Continent> continents);
    int getNumberOfArmies();
    void placeArmies();

private:
    int* numberOfArmies;
    Player* player;
    std::vector<Map::Country>* countries;
    std::vector<Map::Continent>* continents;
    int countriesOwned();
    int continentControlValue();
    int cardExchange();
};

#endif //COMP_345_PROJ_REINFORCEMENTPHASE_H
