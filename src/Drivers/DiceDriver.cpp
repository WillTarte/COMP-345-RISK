#include <iostream>
#include "../../include/Dice.h"

int main() {
  return test_requestMultipleDice();
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
