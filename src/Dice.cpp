#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include "../include/Dice.h"

////////////////////////////////////////////////////////////
/////////////////      DICEROLLER METHODS      /////////////
////////////////////////////////////////////////////////////
/**
 * DiceRoller constructor
 * @param numDice
 */
DiceRoller::DiceRoller(int n) {
    numDice = n;
    dice = new std::vector<Dice>();

    for (int i = 0; i < numDice; i++) {
        dice->push_back(*new Dice());
    }
}

/**
 * Roll 3 dice and return the results in sorted order
 */
std::vector<u_int> DiceRoller::roll() {
    auto* results = new std::vector<u_int>();
    for (int i = 0; i < numDice; i++) {
        results->push_back(dice->at(i).roll());
    }

    std::sort(results->begin(), results->end());
    return *results;
}

/**
 * Return the individual history for each of the dice
 */
std::vector<std::vector<u_int>> DiceRoller::getHistory() {
    return {
        dice->at(0).getHistory(),
        dice->at(1).getHistory(),
        dice->at(2).getHistory()
    };
}

/**
 * Get the number of dice initialized for this player
 */
u_int DiceRoller::getNumDice() {
    return numDice;
}

/**
 * Get each of the dice objects
 */
std::vector<Dice> DiceRoller::getDice() {
    return *dice;
}

/////////////////////////////////////////////////////////////
////////////////////    DICE METHODS    /////////////////////
/////////////////////////////////////////////////////////////
/**
 * Dice constructor
 */
Dice::Dice() {
    history = new std::vector<u_int>();
    for (int i = 0; i < 6; i++) {
        history->push_back(0);
    }

    timesRolled = 0;

}

/**
 * Return all the past rolls in order
 */
std::vector<u_int> Dice::getHistory() {
    return *history;
}

/**
 * Save the roll in the vector storing it's history
 * @param {int} roll
 */
void Dice::saveRoll(u_int roll) {
    (*history)[roll-1]++;
}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
u_int Dice::roll() {
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 eng{seed};
    std::uniform_int_distribution<> dist(1,6);

    u_int roll = dist(eng);
    saveRoll(roll);

    return roll;
}
