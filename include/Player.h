//
// Created by Willi on 9/21/2019.
//

#include <vector>
#include "Map.h"
#include "Cards.h"

#ifndef COMP_345_PROJ_PLAYER_H
#define COMP_345_PROJ_PLAYER_H

class Player {

public:
    void reinforce(Map::Country *fromCountry, Map::Country *toCountry, int numArmies);

    void attack(Map::Country *fromCountry, Map::Country *toCountry, const Player *defendingPlayer);

    void fortify(Map::Country *countryToFortify, int numArmies);

    Hand getHand() { return *cardsPointer; }

    std::vector<Map::Country *> getOwnedCountries() { return *ownedCountriesPointer; }
    // Dice* getDice();

private:
    std::vector<Map::Country *> *ownedCountriesPointer;
    Hand *cardsPointer;
    //  Dice* dicePointer;
    const int *playerId;
};

#endif //COMP_345_PROJ_PLAYER_H
