#include <random>
#include <vector>
#include <algorithm>
#include "../include/Dice.h"

////////////////////////////////////////////////////////////
/////////////////      DICEROLLER METHODS      /////////////
////////////////////////////////////////////////////////////
/**
 * DiceRoller constructor
 * @param numDice
 */
DiceRoller::DiceRoller(): diceRolled(new int(0)), history(new std::vector<int>) {
    for (int x = 0 ; x < 6 ; x++) {
        history->push_back(0);
    }
}

/**
 * Roll <n> dice and return the results in sorted order
 */
std::vector<int> DiceRoller::roll(int numDice) {
    auto* results = new std::vector<int>();
    Dice d = Dice();
    for (int i = 0; i < numDice; i++) {
        int result = d.roll();
        results->push_back(result);
        saveRoll(result);
    }

    std::sort(results->begin(), results->end());
    return *results;
}

void DiceRoller::saveRoll(int roll) {
    (*history)[roll-1]++;
    (*diceRolled)++;
}

/////////////////////////////////////////////////////////////
////////////////////    DICE METHODS    /////////////////////
/////////////////////////////////////////////////////////////
/**
 * Dice constructor
 */
Dice::Dice() {}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
int Dice::roll() {
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng{seed};
    std::uniform_int_distribution<> dist(1,6);

    int roll = dist(eng);

    return roll;
}
