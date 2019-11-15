#include <iostream>
#include "../include/PlayerStrategy.h"

char HumanPlayerStrategy::yesOrNo(StrategyContext _) {
    /*
     * Need the parameter to generatlize the function, but human inputs
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
            attackNewArmies();
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
        }
    }

    return count;
}

int AggressiveBotStrategy::attackFromCountryIndex(int except) {
    int biggestIndex = 0;
    int max = 0;
    auto countries = player->getOwnedCountries();
    for (auto i = 0; i < countries->size(); i++) {
        if (countries[i][0]->getNumberOfTroops() > max
         && countries[i][0]->getCountryId() != except) { // Allows you to find 2nd biggest
            max = countries[i][0]->getNumberOfTroops();
            biggestIndex = countries[i][0]->getCountryId();
            from = *countries[i][0];
        }
    }

    return biggestIndex;
};

int AggressiveBotStrategy::attackToCountryIndex() {
    auto adj = from.getAdjCountries();
    int smallest = INT64_MAX;
    int smallestIndex = -1;
    int owner = from.getPlayerOwnerID();
    for (int i = 0 ; i < adj->size() ; i++) {
        if (adj[i][0]->getNumberOfTroops() < smallest
         && adj[i][0]->getPlayerOwnerID() != owner) {
            smallest = adj[i][0]->getNumberOfTroops();
            smallestIndex = i;
        }
    }

    if (smallestIndex != -1) {
        return smallest;
    }
    else {
        // Get the next smallest country if attacking player
        // owns all adjacent countries
        attackFromCountryIndex(from.getCountryId());
        attackToCountryIndex();
    }
};

int AggressiveBotStrategy::attackNewArmies() {};

int AggressiveBotStrategy::attackNumDice() {};

int AggressiveBotStrategy::defendNumDice() {};

int AggressiveBotStrategy::fortifyFromCountryIndex() {};

int AggressiveBotStrategy::fortifyToCountryIndex() {};

int AggressiveBotStrategy::fortifyArmyCount() {};

int AggressiveBotStrategy::numArmies() {};

int AggressiveBotStrategy::place() {};

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
        }
    }

    return botChoice;
}

int BenevolentBotStrategy::intInput(StrategyContext context) {
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
            attackNewArmies();
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
        }
    }

    return count;
}

int BenevolentBotStrategy::attackFromCountryIndex() {};

int BenevolentBotStrategy::attackToCountryIndex() {};

int BenevolentBotStrategy::attackNewArmies() {};

int BenevolentBotStrategy::attackNumDice() {};

int BenevolentBotStrategy::defendNumDice() {};

int BenevolentBotStrategy::fortifyFromCountryIndex() {};

int BenevolentBotStrategy::fortifyToCountryIndex() {};

int BenevolentBotStrategy::fortifyArmyCount() {};

int BenevolentBotStrategy::numArmies() {};

int BenevolentBotStrategy::place() {};
