#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"

enum StrategyContext {
    ATTACK,
    ATTACK_FROM_COUNTRY,
    ATTACK_TO_COUNTRY,
    ATTACK_DICE_COUNT,
    ATTACK_NEW_ARMY_COUNT,
    DEFEND_DICE_COUNT,
    FORTIFY,
    FORTIFY_FROM_COUNTRY,
    FORTIFY_TO_COUNTRY,
    FORTIFY_ARMY_COUNT,
    REINFORCE,
    REINFORCE_CARD_COUNT,
    REINFORCE_ARMY_COUNT
};

#ifndef COMP_345_PROJ_PLAYER_STRATEGY_H
#define COMP_345_PROJ_PLAYER_STRATEGY_H

class PlayerStrategy {
public:
    virtual char yesOrNo(StrategyContext context) = 0;
    virtual int intInput(StrategyContext context) = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    char yesOrNo(StrategyContext _);
    int intInput(StrategyContext _);
};

class AggressiveBotStrategy : public PlayerStrategy {
public:
    char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);

private:
    int attackFromCountryIndex();
    int attackToCountryIndex();
    int attackNewArmies();
    int attackNumDice();
    int defendNumDice();
    int fortifyFromCountryIndex();
    int fortifyToCountryIndex();
    int fortifyArmyCount();
    int numArmies();
    int place();
};

class BenevolentBotStrategy : public PlayerStrategy {
public:
    char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);

private:
    int attackFromCountryIndex();
    int attackToCountryIndex();
    int attackNewArmies();
    int attackNumDice();
    int defendNumDice();
    int fortifyFromCountryIndex();
    int fortifyToCountryIndex();
    int fortifyArmyCount();
    int numArmies();
    int place();
};

#endif //COMP_345_PROJ_PLAYER_STRATEGY_H
