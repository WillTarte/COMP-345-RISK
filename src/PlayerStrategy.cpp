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
            break;
        default: {
        }
    }

    return count;
}

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
            break;
        default: {
        }
    }

    return count;
}
