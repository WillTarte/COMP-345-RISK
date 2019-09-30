#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include "../include/Dice.h"

/////////////////////////////////////////////////////////////
/////////////////      DICEROLLER METHODS      //////////////
/////////////////////////////////////////////////////////////
/**
 * DiceRoller constructor
 * @param numDice
 */
DiceRoller::DiceRoller(u_int _numDice) {
    numDice = &_numDice;
    dice = {};

    Dice* d = new Dice();
    for (u_int i = 0; i < *numDice; i++) {
        std::cout << "Here" << std::endl;
        dice->push_back(*d);
    }
}

/**
 * Roll 3 dice and return the results in sorted order
 */
std::vector<u_int> DiceRoller::roll() {
    std::vector<u_int> results = {0, 0, 0};
    for (u_int i = 0; i < *numDice; i++) {
        results[i] = dice->at(i).roll();
    }

    return results;
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
    return *numDice;
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
    history = {};
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
    history->push_back(roll);
}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
u_int Dice::roll() {
    srand(time(nullptr));
    u_int roll = rand() % 6 + 1;
    saveRoll(roll);

    return roll;
}
