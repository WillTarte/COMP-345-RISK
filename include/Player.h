//
// Created by William Tarte on 9/21/2019.
//

#include <vector>
#include <list>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"
#include "PlayerStrategy.h"

#ifndef COMP_345_PROJ_PLAYER_H
#define COMP_345_PROJ_PLAYER_H

class Observer;

enum PlayerState {
    ATTACKING,
    DEFENDING,
    FORTIFYING,
    REINFORCING,
    IDLE
};

std::ostream& operator<<(std::ostream& os, const PlayerState state);

class Player {
public:
    Player(std::vector<Map::Country*>* ownedCountries, Hand* cards, DiceRoller* diceRoller, int playerId);
    ~Player();
    Player(const Player& toCopy);
    void operator=(const Player& rhs);
    int reinforce();
    int attack();
    int fortify();
    void notifyAll();
    void attachObserver(Observer* observer);
    void detachObserver(Observer* toDetach);

    inline Hand* getCards() { return pCards; }
    inline std::vector<Map::Country*>* getOwnedCountries() { return pOwnedCountries; }
    inline void setOwnedCountries(std::vector<Map::Country*>* countries){pOwnedCountries = countries;}

    inline DiceRoller* getDiceRoller() { return pDiceRoller; }
    inline PlayerStrategy* getStrategy() { return strategy; }
    inline int getPlayerId() const { return *pPlayerId; }

    inline PlayerState getPlayerState() const { return *currentState; }
    inline void setPlayerState(PlayerState newState) { delete currentState; this->currentState =  new PlayerState(newState);}
    void setPlayerStrategy(Strategies strategy);

private:
    int executeAttack(Map::Country* fromCountry, Map::Country* toCountry, Player* defendingPlayer, int numAttackingDice, int numDefendingDice);
    int executeFortify(Map::Country& fromCountry, Map::Country& countryToFortify, int numArmies);
    std::vector<Map::Country*>* pOwnedCountries;
    Hand* pCards;
    DiceRoller* pDiceRoller;
    PlayerStrategy* strategy;
    const int* pPlayerId;
    PlayerState* currentState;
    std::list<Observer*>* pObservers;
};

enum PlayerAction {
    FAILED = -1,
    SUCCEEDED = 1,
    ABORTED = 0
};

#endif //COMP_345_PROJ_PLAYER_H
