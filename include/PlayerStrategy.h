#ifndef COMP_345_PROJ_PLAYER_STRATEGY_H
#define COMP_345_PROJ_PLAYER_STRATEGY_H

#include "Map.h"

class Player;

enum Strategies {
    HUMAN_PLAYER,
    AGGRESSIVE_BOT,
    BENEVOLENT_BOT
};

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

class PlayerStrategy {
public:
    PlayerStrategy();
    PlayerStrategy(const PlayerStrategy& toCopy);
    PlayerStrategy& operator=(const PlayerStrategy& rhs);
    PlayerStrategy(Player* player);
    virtual char yesOrNo(StrategyContext context) = 0;
    virtual int intInput(StrategyContext context) = 0;
    virtual inline void setArmiesToPlace(int armies) { armiesToPlace = new int(armies); }
    virtual inline void setExchangingCardType(int count) { exchangingCardType = new int(count); }
    virtual inline void setTo(Map::Country* toCountry) { to = toCountry; }
    virtual void resetChoices();
    virtual ~PlayerStrategy();

protected:
    int* armiesToPlace;
    int* exchangingCardType;
    int* numWeakest = new int(-1);
    Player* player;
    Map::Country* from;
    Map::Country* to;

};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(Player* player): PlayerStrategy(player){};
    ~HumanPlayerStrategy() = default;
    HumanPlayerStrategy(const HumanPlayerStrategy& toCopy);
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& rhs);
    HumanPlayerStrategy(const Player& player);
    char yesOrNo(StrategyContext _);
    int intInput(StrategyContext _);
};

class AggressiveBotStrategy : public PlayerStrategy {
public:
    AggressiveBotStrategy();
    AggressiveBotStrategy(Player* player): PlayerStrategy(player){};
    AggressiveBotStrategy(const Player &player);
    ~AggressiveBotStrategy() = default;
    AggressiveBotStrategy(const AggressiveBotStrategy& toCopy);
    AggressiveBotStrategy& operator=(const AggressiveBotStrategy& rhs);
    char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);

private:
    bool willAttack();
    bool canFortify();
    int attackFromCountryIndex();
    int attackToCountryIndex();
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
    BenevolentBotStrategy();
    BenevolentBotStrategy(Player* player): PlayerStrategy(player){};
    BenevolentBotStrategy(const Player &player);
    ~BenevolentBotStrategy() = default;
    BenevolentBotStrategy(const BenevolentBotStrategy& toCopy);
    BenevolentBotStrategy& operator=(const BenevolentBotStrategy& rhs);
    char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);

private:
    bool canFortify();
    int defendNumDice();
    int fortifyFromCountryIndex();
    int fortifyToCountryIndex();
    int fortifyArmyCount();
    int numArmies();
    int place();
};

#endif //COMP_345_PROJ_PLAYER_STRATEGY_H
