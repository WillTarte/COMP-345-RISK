#include <iostream>
#include "../../include/Dice.h"

int main() {
  return
    test_requestMultipleDice() &&
    test_returnCorrectNumOfDice() &&
    test_valuesInCorrectRange();
}

/**
 * Test to make sure that a user can create a dice roller with between 1 and 3 dice
 */
bool test_requestMultipleDice() {
  bool success = true;

  for (int i = 1; i < 4; i++) {
    DiceRoller roller = DiceRoller(i);

    try {
      DiceRoller roller = DiceRoller(i);
    }
    catch (int e) {
      success = false;
    }
  }

  return success;
}

/**
 * Test to ensure that regardless of no failure, the dice are created.
 */
bool test_returnCorrectNumOfDice() {
  bool success = true;

  for (int i = 1; i < 4; i++) {
    DiceRoller roller = DiceRoller(i);
    int length = roller.getDice()->size();

    success = sucess && lenght == i;
  }

  return success;
}

/**
 * Test to make sure all numbers resulting from rolls are between 1 and 6 (inclusive)
 */
bool test_valuesInCorrectRange() {
  bool success = true;

  Dice dice = Dice();
  for (int i = 1; i < 100; i++) {
    int result = dice.roll();
    success = sucess && (result < 7 && result > 0);
  }

  return success;
}

/**
 * Test to make sure the rolling of the dice is "fair"
 */
bool test_equalShareOfValues() {
  return false;
}

/**
 * Test to ensure the history of rolls is properly remembered
 */
bool test_maintainPercentageOfRolls() {
  return false;
}
