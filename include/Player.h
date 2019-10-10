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
    Player(std::vector<Map::Country*> ownedCountries, Hand cards, DiceRoller diceRoller, int playerId);

    int reinforce(Map::Country& fromCountry, Map::Country& toCountry, int numArmies);

    int attack(Map::Country& fromCountry, Map::Country& toCountry, Player& defendingPlayer, int numAttackingDice,
               int numDefendingDice);

    int fortify(Map::Country& countryToFortify, int numArmies);

    Hand& getCards() { return *pCards; }

    std::vector<Map::Country*>& getOwnedCountries() { return *pOwnedCountries; }

    DiceRoller& getDiceRoller() { return *pDiceRoller; }

    int getPlayerId() { return *pPlayerId; }

private:
    std::vector<Map::Country*>* pOwnedCountries;
    Hand* pCards;
    DiceRoller* pDiceRoller;
    const int* pPlayerId;
};

enum PlayerAction {
    FAILED = -1,
    SUCCEEDED = 1,
};

#endif //COMP_345_PROJ_PLAYER_H
