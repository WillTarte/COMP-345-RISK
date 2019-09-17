#include <iostream>
#include "../../include/Dice.h"

int main() {
  return test_requestMultipleDice() &&
    test_returnCorrectNumOfDice();
}

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

bool test_returnCorrectNumOfDice() {
  bool success = true;

  for (int i = 1; i < 4; i++) {
    DiceRoller roller = DiceRoller(i);
    int length = roller.getDice()->size();

    success = sucess && lenght == i;
  }

  return success;
}

bool test_valuesInCorrectRange() {
  return false;
}

bool test_equalShareOfValues() {
  return false;
}

bool test_maintainPercentageOfRolls() {
  return false;
}
