#include <iostream>
#include "../../include/Dice.h"

bool test_DiceRoller() {
    bool success = true;

    for (u_int i = 1; i < 4; i++) {
        try {
            DiceRoller roller = DiceRoller(i);
            success = success && roller.getNumDice() == i;
            //success = success && roller.getDice().size() == i;
        }
        catch (int e) {
            success = false;
        }
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
    success = success && (result < 7 && result > 0);
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

void assert(bool success) {
    if (success)
        std::cout << "Pass" << std::endl;
    else
        std::cout << "Fail" << std::endl;
}

int main() {
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------- Running tests for Dice class -------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "----- Testing Dice constructor" << std::endl;
    assert(test_DiceRoller());

    std::cout << "" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "---------- Running tests for DiceRoller class ----------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "----- Testing DiceRoller constructor" << std::endl;
    //std::cout << (test_DiceRoller() ? "Pass" : "Fail") << std::endl;

    std::cout << "----- Testing roll method" << std::endl;
    //std::cout << (test_DiceRoller() ? "Pass" : "Fail") << std::endl;

    std::cout << "----- Testing getHistory" << std::endl;
    //std::cout << (test_DiceRoller() ? "Pass" : "Fail") << std::endl;

    std::cout << "----- Testing getDice" << std::endl;
    //std::cout << (test_DiceRoller() ? "Pass" : "Fail") << std::endl;

    std::cout << "----- Testing getNumDice" << std::endl;
    //std::cout << (test_DiceRoller() ? "Pass" : "Fail") << std::endl;

    return 0;
}
