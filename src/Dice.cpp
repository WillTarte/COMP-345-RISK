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
DiceRoller::DiceRoller(int _numDice): numDice(new int(_numDice)), dice(new std::vector<Dice>()) {
    for (int i = 0; i < *numDice; i++) {
        dice->push_back(*new Dice());
    }
}

/**
 * Roll <n> dice and return the results in sorted order
 */
std::vector<int> DiceRoller::roll() {
    auto* results = new std::vector<int>();
    for (int i = 0; i < *numDice; i++) {
        results->push_back(dice->at(i).roll());
    }

    std::sort(results->begin(), results->end());
    return *results;
}

/**
 * Return the individual history for each of the dice
 */
std::vector<std::vector<int>> DiceRoller::getHistory() {
    return {
        dice->at(0).getHistory(),
        dice->at(1).getHistory(),
        dice->at(2).getHistory()
    };
}

/////////////////////////////////////////////////////////////
////////////////////    DICE METHODS    /////////////////////
/////////////////////////////////////////////////////////////
/**
 * Dice constructor
 */
Dice::Dice(): timesRolled(nullptr), history(new std::vector<int>()) {
    for (int i = 0; i < 6; i++) {
        history->push_back(0);
    }
}

/**
 * Save the roll in the vector storing it's history
 * @param {int} roll
 */
void Dice::saveRoll(int roll) {
    (*history)[roll-1]++;
}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
int Dice::roll() {
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng{seed};
    std::uniform_int_distribution<> dist(1,6);

    int roll = dist(eng);
    saveRoll(roll);

    return roll;
}
