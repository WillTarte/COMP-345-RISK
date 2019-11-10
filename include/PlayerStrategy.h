#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

#ifndef COMP_345_PROJ_PLAYER_STRATEGY_H
#define COMP_345_PROJ_PLAYER_STRATEGY_H

class PlayerStrategy {
public:
    virtual int attack() = 0;
    virtual int fortify() = 0;
    virtual int reinforce() = 0;
};

class HumanPlayerStrategy: public PlayerStrategy {
    int attack();
    int fortify();
    int reinforce();
};

class AggressiveBotStrategy: public PlayerStrategy {
    int attack();
    int fortify();
    int reinforce();
};

class BenevolentBotStrategy: public PlayerStrategy {
    int attack();
    int fortify();
    int reinforce();
};

#endif //COMP_345_PROJ_PLAYER_STRATEGY_H
