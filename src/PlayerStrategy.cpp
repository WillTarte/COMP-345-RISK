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

int AggressiveBotStrategy::attackFromCountryIndex() {};

int AggressiveBotStrategy::attackToCountryIndex() {};

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
