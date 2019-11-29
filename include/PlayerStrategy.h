#ifndef COMP_345_PROJ_PLAYER_STRATEGY_H
#define COMP_345_PROJ_PLAYER_STRATEGY_H

#include "Map.h"
#include <random>
#include <algorithm>

class Player;

enum Strategies {
    HUMAN_PLAYER,
    AGGRESSIVE_BOT,
    BENEVOLENT_BOT,
    RANDOM_BOT,
    CHEATER_BOT
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
    explicit PlayerStrategy(Player* player);
    virtual signed char yesOrNo(StrategyContext context) = 0;
    virtual int intInput(StrategyContext context) = 0;
    virtual inline void setArmiesToPlace(int armies) { armiesToPlace = new int(armies); }
    virtual inline void setExchangingCardType(int count) { exchangingCardType = new int(count); }
    virtual inline void setTo(Map::Country* toCountry) { to = toCountry; }
    virtual void resetChoices();
    virtual ~PlayerStrategy();
    std::string getStrategyName() const { return *strategyName; };

protected:
    int* armiesToPlace;
    int* exchangingCardType;
    int* numWeakest = new int(-1);
    Player* player;
    Map::Country* from;
    Map::Country* to;
    const std::string* strategyName;

};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    explicit HumanPlayerStrategy(Player* player): PlayerStrategy(player) { this->strategyName = new std::string("HUMAN"); };
    ~HumanPlayerStrategy() = default;
    HumanPlayerStrategy(const HumanPlayerStrategy& toCopy);
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& rhs);
    HumanPlayerStrategy(const Player& player);
    signed char yesOrNo(StrategyContext _);
    int intInput(StrategyContext _);
};

class AggressiveBotStrategy : public PlayerStrategy {
public:
    AggressiveBotStrategy();
    explicit AggressiveBotStrategy(Player* player): PlayerStrategy(player) { this->strategyName = new std::string("AGGRESSIVE"); };
    explicit AggressiveBotStrategy(const Player &player);
    ~AggressiveBotStrategy() = default;
    AggressiveBotStrategy(const AggressiveBotStrategy& toCopy);
    AggressiveBotStrategy& operator=(const AggressiveBotStrategy& rhs);
    signed char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);

private:
    bool canAttack();
    bool canFortify();
    int attackFromCountryIndex();
    int attackToCountryIndex();
    int attackNumDice();
    int defendNumDice();
    int fortifyFromCountryIndex();
    int fortifyToCountryIndex();
    int fortifyArmyCount();
    int numCards();
    int reinforceArmyCount();
};

class BenevolentBotStrategy : public PlayerStrategy {
public:
    BenevolentBotStrategy();
    explicit BenevolentBotStrategy(Player* player): PlayerStrategy(player){ this->strategyName = new std::string("BENEVOLENT"); };
    explicit BenevolentBotStrategy(const Player &player);
    ~BenevolentBotStrategy() = default;
    BenevolentBotStrategy(const BenevolentBotStrategy& toCopy);
    BenevolentBotStrategy& operator=(const BenevolentBotStrategy& rhs);
    signed char yesOrNo(StrategyContext context);
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

class RandomBotStrategy : public PlayerStrategy {
public:
    RandomBotStrategy();
    explicit RandomBotStrategy(Player* player) : PlayerStrategy(player){ this->strategyName = new std::string("RANDOM"); };
    explicit RandomBotStrategy(const Player &player);
    ~RandomBotStrategy() = default;
    RandomBotStrategy(const RandomBotStrategy& toCopy);
    RandomBotStrategy& operator=(const RandomBotStrategy& rhs);
    signed char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);
private:
    int chooseRandomCountry(std::mt19937 gen);
    int chooseRandomNeighbour(std::mt19937 gen);
    int sendRandomArmies(std::mt19937 gen);
    int attackMaxDice(std::mt19937 gen);
    int reinforceMaxCards(std::mt19937 gen, int currentType);
};

class CheaterBotStrategy : public PlayerStrategy {
public:
    CheaterBotStrategy();
    explicit CheaterBotStrategy(Player* player) : PlayerStrategy(player) { this->strategyName = new std::string("CHEATER"); };
    explicit CheaterBotStrategy(const Player& player);
    ~CheaterBotStrategy() = default;
    CheaterBotStrategy(const CheaterBotStrategy& toCopy);
    CheaterBotStrategy& operator=(const CheaterBotStrategy& rhs);
    signed char yesOrNo(StrategyContext context);
    int intInput(StrategyContext context);
    int cheaterReinforce();
    int cheaterAttack();
    int cheaterFortify();
private:
    bool exchangeCountryOwnership(Player* defendingPlayer, Map::Country* toExchange);
};

#endif //COMP_345_PROJ_PLAYER_STRATEGY_H
