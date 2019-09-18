//
// Created by Philip on 9/14/2019.
//

#include <random>
#include <vector>
#include "../include/Dice.h"

/**
 * DiceRoller constructor
 * @param numDice
 */
DiceRoller::DiceRoller(int numDice) {
  dice = new std::vector<Dice>;

  for (int i = 0; i < numDice; i++) {
    dice.push_back(new Dice());
  }
}

/**
 * Roll 3 dice and return the results in sorted order
 */
DiceRoller::roll() {
  int[] results = {0, 0, 0};
  for (int i = 0; i < numDice; i++) {
    results[i] = dice[i].roll();
  }

  return std::sort(results);
}

/**
 * Dice constructor
 */
Dice::Dice() {
  history = {};
}

/**
 * Return all the past rolls in order
 */
std::vector Dice::getHistory() {
  return *history;
}

/**
 * Save the roll in the vector storing it's history
 * @param {int} roll
 */
void Dice::saveRoll(int roll) {
  history.push_back(roll);
}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
int Dice::roll() {
  srand(time(NULL));
  int roll = rand() % 6 + 1;
  saveRoll(roll);

  return roll;
}
