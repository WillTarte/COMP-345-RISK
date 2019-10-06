//
// Created by William Tarte on 9/21/2019.
//

#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

#ifndef COMP_345_PROJ_PLAYER_H
#define COMP_345_PROJ_PLAYER_H

class Player {

public:
    void reinforce(Map::Country *fromCountry, Map::Country *toCountry, int numArmies);

    void attack(Map::Country *fromCountry, Map::Country *toCountry, Player *defendingPlayer, int numAttackingDice,
                int numDefendingDice);

    void fortify(Map::Country *countryToFortify, int numArmies);

    Hand getHand() { return *cardsPointer; }
    std::vector<Map::Country *> getOwnedCountries() { return *ownedCountriesPointer; }

    DiceRoller getDiceRoller() { return *diceRoller; }

    int getPlayerId() { return *playerId; }

private:
    std::vector<Map::Country *> *ownedCountriesPointer;
    Hand *cardsPointer;
    DiceRoller *diceRoller;
    const int *playerId;
};

#endif //COMP_345_PROJ_PLAYER_H
