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
    Player(const Player& other);
    void operator=(const Player& rhs);
    ~Player() {
        delete pOwnedCountries;
        delete pCards;
        delete pDiceRoller;
        delete pPlayerId;
    }

    int reinforce();
    int attack();
    int fortify();

    /* Getters & Setters */
    Hand& getCards() { return *pCards; }
    std::vector<Map::Country*>* getOwnedCountries() { return pOwnedCountries; }
    void setOwnedCountries(std::vector<Map::Country*>* countries){pOwnedCountries = countries;}
    DiceRoller& getDiceRoller() { return *pDiceRoller; }
    int getPlayerId() { return *pPlayerId; }

private:
    // Helper Method for attacking
    int executeAttack(Map::Country& fromCountry, Map::Country& toCountry, Player& defendingPlayer, int numAttackingDice, int numDefendingDice);
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
