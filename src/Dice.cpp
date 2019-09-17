//
// Created by Philip on 9/14/2019.
//

#include <random>
#include <vector>
#include "../include/Dice.h"

/**
 * Dice constructor
 * @param numDice
 */
Dice::DiceRoller(int numDice) {
  dice = new std::vector<Dice>;

  for (int i = 0; i < numDice; i++) {
    dice.push_back(new Dice());
  }
}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
Dice::roll() {
  srand(time(NULL));
  return rand() % Dice::sides+1;
}
