#include <iostream>
#include "../include/Player.h"
#include "../include/PlayerStrategy.h"
#include "../include/Map.h"

/******************************************************************************/
/*********************** Player strategy constructors *************************/
/******************************************************************************/
PlayerStrategy::PlayerStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

PlayerStrategy::PlayerStrategy(const Player& player) {
    this->exchangingCardType = new int(0);
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/*********************** Player strategy constructors *************************/
/******************************************************************************/
HumanPlayerStrategy::~HumanPlayerStrategy() {
    delete armiesToPlace;
    delete player;
    delete from;
    delete to;
}

HumanPlayerStrategy::HumanPlayerStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

HumanPlayerStrategy::HumanPlayerStrategy(const Player& player) {
    this->exchangingCardType = new int(0);
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/*********************** Aggressive bot constructors **************************/
/******************************************************************************/
AggressiveBotStrategy::~AggressiveBotStrategy() {
    delete armiesToPlace;
    delete player;
    delete from;
    delete to;
}

AggressiveBotStrategy::AggressiveBotStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

AggressiveBotStrategy::AggressiveBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

AggressiveBotStrategy::AggressiveBotStrategy(const AggressiveBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

AggressiveBotStrategy& AggressiveBotStrategy::operator=(const AggressiveBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/*********************** Benevolent bot constructors **************************/
/******************************************************************************/
BenevolentBotStrategy::~BenevolentBotStrategy() {
    delete armiesToPlace;
    delete player;
    delete from;
    delete to;
}

BenevolentBotStrategy::BenevolentBotStrategy() {
    this->armiesToPlace = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

BenevolentBotStrategy::BenevolentBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

BenevolentBotStrategy::BenevolentBotStrategy(const BenevolentBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

BenevolentBotStrategy& BenevolentBotStrategy::operator=(const BenevolentBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/******************** Human player strategy methods ***************************/
/******************************************************************************/
char HumanPlayerStrategy::yesOrNo(StrategyContext _) {
    /*
     * Need the parameter to generalize the function, but human inputs
     * don't need context. If you don't do anything with the parameter,
     * it throws a warning. So this way we get to generalize the solution
     * by just throwing away the parameter immediately.
     */
    delete &_;
    char playerDecision = 0;
    std::cin >> playerDecision;

    return playerDecision;
}

int HumanPlayerStrategy::intInput(StrategyContext _) {
    delete &_;
    int count = 0;
    std::cin >> count;

    return count;
}

/******************************************************************************/
/******************* Aggressive bot strategy methods **************************/
/******************************************************************************/
char AggressiveBotStrategy::yesOrNo(StrategyContext context) {
    char botChoice = 0;
    switch ((int) context) {
        case StrategyContext::ATTACK:
            botChoice = 'y';
            break;
        case StrategyContext::FORTIFY:
            botChoice = 'y';
            break;
        case StrategyContext::REINFORCE:
            botChoice = 'y';
            break;
        default: {
            // Should never occur
            botChoice = 'n';
        }
    }

    return botChoice;
}

int AggressiveBotStrategy::intInput(StrategyContext context) {
    int count = 0;

    switch ((int) context) {
        case StrategyContext::ATTACK_FROM_COUNTRY:
            count = attackFromCountryIndex();
            break;
        case StrategyContext::ATTACK_TO_COUNTRY:
            count = attackToCountryIndex();
            break;
        case StrategyContext::ATTACK_DICE_COUNT:
            attackNumDice();
            break;
        case StrategyContext::ATTACK_NEW_ARMY_COUNT:
            count = 1;
            break;
        case StrategyContext::DEFEND_DICE_COUNT:
            defendNumDice();
            break;
        case StrategyContext::FORTIFY_FROM_COUNTRY:
            fortifyFromCountryIndex();
            break;
        case StrategyContext::FORTIFY_TO_COUNTRY:
            fortifyToCountryIndex();
            break;
        case StrategyContext::FORTIFY_ARMY_COUNT:
            fortifyArmyCount();
            break;
        case StrategyContext::REINFORCE_ARMY_COUNT:
            numArmies();
            break;
        case StrategyContext::REINFORCE_CARD_COUNT:
            place();
            break;
        default: {
            count = -1;
        }
    }

    return count;
}

/*
 * Just find your biggest country and attack with it.
 * The except parameter is in case you own all countries adjacent to your biggest.
 **/
int AggressiveBotStrategy::attackFromCountryIndex(int except) {
    int biggestIndex = 0;
    int max = 0;
    auto countries = player->getOwnedCountries();
    for (unsigned long i = 0; i < countries->size(); i++) {
        if (countries[i][0]->getNumberOfTroops() > max
         && countries[i][0]->getCountryId() != except) { // Allows you to find 2nd biggest
            max = countries[i][0]->getNumberOfTroops();
            biggestIndex = countries[i][0]->getCountryId();
            from = countries[i][0];
        }
    }

    return biggestIndex;
}

int AggressiveBotStrategy::attackToCountryIndex() {
    auto adj = from->getAdjCountries();
    int smallest = INT32_MAX;
    unsigned long smallestIndex = 0;
    int owner = from->getPlayerOwnerID();
    Map::Country* smallestCountry = nullptr;
    for (unsigned long i = 0 ; i < adj->size() ; i++) {
        if (adj[i][0]->getNumberOfTroops() < smallest
         && adj[i][0]->getPlayerOwnerID() != owner) {
            smallest = adj[i][0]->getNumberOfTroops();
            smallestIndex = i;
            smallestCountry = adj[i][0];
        }
    }

    if (smallestIndex != 0) {
        to = smallestCountry;
        return smallest;
    }
    else {
        // Get the next smallest country if attacking player
        // owns all adjacent countries
        attackFromCountryIndex(from->getCountryId());
        return attackToCountryIndex();
    }
}

/*
 * Attack with the most number of dice they can. Uses the same logic to find the upper
 * limits of how many dice they can attack with, and attacks with as much as possible.
 **/
int AggressiveBotStrategy::attackNumDice() {
    int numDice = 0;
    if (from->getNumberOfTroops() > 3) {
        numDice = 3;
    }
    else {
        numDice = from->getNumberOfTroops() - 1;
    }

    return numDice;
}

/*
 * Defend with the maximum number of dice.
 * TODO: Check if this is one of the player's smaller countries, then defend the minimum?
 **/
int AggressiveBotStrategy::defendNumDice() {
    int numDice = 0;
    if (to->getNumberOfTroops() >= 2) {
        numDice = 2;
    }
    else {
        numDice = 1;
    }

    return numDice;
}

/*
 * Find the smallest country the player owns and fortify that.
 **/
int AggressiveBotStrategy::fortifyFromCountryIndex() {
    from = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (from == nullptr) {
            from = country;
        }
        else if (country->getNumberOfTroops() < from->getNumberOfTroops()) {
            from = country;
        }
    }

    return from->getCountryId();
}

/*
 * This function is just going to find the biggest country the player owns
 * and use that country as the one to fortify.
 **/
int AggressiveBotStrategy::fortifyToCountryIndex() {
    to = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (to == nullptr) {
            to = country;
        }
        else if (country->getNumberOfTroops() > to->getNumberOfTroops()) {
            to = country;
        }
    }

    return to->getCountryId();
}

/*
 * Just send all your troops from weaker countries to your biggest one to give it
 * as much strength as possible.
 **/
int AggressiveBotStrategy::fortifyArmyCount() {
    return from->getNumberOfTroops() - 1;
}

int AggressiveBotStrategy::numArmies() {
    int count = 0;
    for (auto cardType : *player->getCards()->getHand()) {
        if ((int) cardType == *exchangingCardType) {
            count++;
        }
    }

    return count;
}

int AggressiveBotStrategy::place() {
    Map::Country* biggest = nullptr;
    // Find what the biggest country is
    for (auto* country : *player->getOwnedCountries()) {
        if (country->getNumberOfTroops() > biggest->getNumberOfTroops()) {
            biggest = country;
        }
    }

    if (biggest != nullptr && biggest->getCountryId() == to->getCountryId()) {
        return *armiesToPlace;
    }
    else {
        return 0;
    }
}

/******************************************************************************/
/******************* Benevolent bot strategy methods **************************/
/******************************************************************************/
char BenevolentBotStrategy::yesOrNo(StrategyContext context) {
    char botChoice = 0;
    switch ((int) context) {
        case StrategyContext::ATTACK:
            botChoice = 'n';
            break;
        case StrategyContext::FORTIFY:
            botChoice = 'y';
            break;
        case StrategyContext::REINFORCE:
            botChoice = 'y';
            break;
        default: {
            // Should never occur
            botChoice = 'n';
        }
    }

    return botChoice;
}

int BenevolentBotStrategy::intInput(StrategyContext context) {
    int count = 0;

    switch ((int) context) {
        case StrategyContext::DEFEND_DICE_COUNT:
            defendNumDice();
            break;
        case StrategyContext::FORTIFY_FROM_COUNTRY:
            fortifyFromCountryIndex();
            break;
        case StrategyContext::FORTIFY_TO_COUNTRY:
            fortifyToCountryIndex();
            break;
        case StrategyContext::FORTIFY_ARMY_COUNT:
            fortifyArmyCount();
            break;
        case StrategyContext::REINFORCE_ARMY_COUNT:
            numArmies();
            break;
        case StrategyContext::REINFORCE_CARD_COUNT:
            place();
            break;
        case StrategyContext::ATTACK_FROM_COUNTRY:
        case StrategyContext::ATTACK_TO_COUNTRY:
        case StrategyContext::ATTACK_DICE_COUNT:
        case StrategyContext::ATTACK_NEW_ARMY_COUNT:
        default: {
            count = -1;
        }
    }

    return count;
}


int BenevolentBotStrategy::defendNumDice() {
    int numDice = 0;
    if (to->getNumberOfTroops() >= 2) {
        numDice = 2;
    }
    else {
        numDice = 1;
    }

    return numDice;
}

int BenevolentBotStrategy::fortifyFromCountryIndex() {
    from = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (from == nullptr) {
            from = country;
        }
        else if (country->getNumberOfTroops() > from->getNumberOfTroops()) {
            from = country;
        }
    }

    return from->getCountryId();
}

int BenevolentBotStrategy::fortifyToCountryIndex() {
    to = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (to == nullptr) {
            to = country;
        }
        else if (country->getNumberOfTroops() < to->getNumberOfTroops()) {
            to = country;
        }
    }

    return to->getCountryId();
}

int BenevolentBotStrategy::fortifyArmyCount() {
    int move = from->getNumberOfTroops();
    move = (move - to->getNumberOfTroops()) / 2;

    return move;
}

int BenevolentBotStrategy::numArmies() {
    int count = 0;
    for (auto cardType : *player->getCards()->getHand()) {
        if ((int) cardType == *exchangingCardType) {
            count++;
        }
    }

    return count;
}

int BenevolentBotStrategy::place() {
    if (*numWeakest == -1) {
        int count = 0;
        int weakest = 0;
        // Find what the smallest country is
        for (auto* country : *player->getOwnedCountries()) {
            if (weakest == 0 || country->getNumberOfTroops() < weakest) {
                weakest = country->getNumberOfTroops();
            }
        }

        // Then find out how many other countries have the same number of armies as it
        for (auto* country : *player->getOwnedCountries()) {
            if (country->getNumberOfTroops() == weakest) {
                count++;
            }
        }

        numWeakest = new int(count);
    }

    return *armiesToPlace / *numWeakest;
}
