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

/*
 * Just find your biggest country and attack with it.
 * The except parameter is in case you own all countries adjacent to your biggest.
 **/
int AggressiveBotStrategy::attackFromCountryIndex(int except) {
    int biggestIndex = 0;
    int max = 0;
    auto countries = player->getOwnedCountries();
    for (auto i = 0; i < countries->size(); i++) {
        if (countries[i][0]->getNumberOfTroops() > max
         && countries[i][0]->getCountryId() != except) { // Allows you to find 2nd biggest
            max = countries[i][0]->getNumberOfTroops();
            biggestIndex = countries[i][0]->getCountryId();
            from = countries[i][0];
        }
    }

    return biggestIndex;
};

int AggressiveBotStrategy::attackToCountryIndex() {
    auto adj = from->getAdjCountries();
    int smallest = INT64_MAX;
    int smallestIndex = -1;
    int owner = from->getPlayerOwnerID();
    Map::Country* smallestCountry = nullptr;
    for (int i = 0 ; i < adj->size() ; i++) {
        if (adj[i][0]->getNumberOfTroops() < smallest
         && adj[i][0]->getPlayerOwnerID() != owner) {
            smallest = adj[i][0]->getNumberOfTroops();
            smallestIndex = i;
            smallestCountry = adj[i][0];
        }
    }

    if (smallestIndex != -1) {
        to = smallestCountry;
        delete smallestCountry;
        return smallest;
    }
    else {
        // Get the next smallest country if attacking player
        // owns all adjacent countries
        attackFromCountryIndex(from->getCountryId());
        attackToCountryIndex();
    }
};

/*
 * Send the minimum number of troops to keep them in your biggest country
 **/
int AggressiveBotStrategy::attackNewArmies() {
    return 1;
};

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
};

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
};

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
};

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
};

/*
 * Just send all your troops from weaker countries to your biggest one to give it
 * as much strength as possible.
 **/
int AggressiveBotStrategy::fortifyArmyCount() {
    return from->getNumberOfTroops();
};

/*
 *
 **/
int AggressiveBotStrategy::numArmies() {

};

/*
 **/
int AggressiveBotStrategy::place() {

};

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
