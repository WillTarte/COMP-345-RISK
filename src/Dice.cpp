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
Dice::Dice(int numDice) {
  numDice = new int(numDice);
}

/**
 * Dice constructor
 * @param numDice, numSides
 */
Dice::Dice(int numDice, int numSides) {

}

/**
 * Generates a random number from 1 to 6 to simulate rolling the dice.
 */
Dice::roll() {
  srand(time(NULL));
  return (rand() % Dice::sides+1;
}
