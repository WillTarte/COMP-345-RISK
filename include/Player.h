//
// Created by William Tarte on 9/21/2019.
//

#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

#ifndef COMP_345_PROJ_PLAYER_H
#define COMP_345_PROJ_PLAYER_H

// TODO: handle actions on unowned neighbouring countries
class Player {
public:
    Player(std::vector<Map::Country*> ownedCountries, Hand* cards, DiceRoller* diceRoller, int playerId);
    ~Player();
    Player(const Player& toCopy);
    void operator=(const Player& rhs);
    int reinforce();
    int attack();
    int fortify();

    inline Hand* getCards() { return pCards; }
    inline std::vector<Map::Country*>* getOwnedCountries() const { return pOwnedCountries; }
    inline void setOwnedCountries(std::vector<Map::Country*>* countries){pOwnedCountries = countries;}

    inline DiceRoller* getDiceRoller() { return pDiceRoller; }

    inline int getPlayerId() const { return *pPlayerId; }

private:
    int executeAttack(Map::Country* fromCountry, Map::Country* toCountry, Player* defendingPlayer, int numAttackingDice, int numDefendingDice);
    int executeFortify(Map::Country& fromCountry, Map::Country& countryToFortify, int numArmies);
    std::vector<Map::Country*>* pOwnedCountries;
    Hand* pCards;
    DiceRoller* pDiceRoller;
    const int* pPlayerId;
};

enum PlayerAction {
    FAILED = -1,
    SUCCEEDED = 1,
    ABORTED = 0
};

#endif //COMP_345_PROJ_PLAYER_H
