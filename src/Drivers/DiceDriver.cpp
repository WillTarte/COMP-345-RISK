#include <iostream>
#include "../../include/Dice.h"

////////////////////////////////////////////////////////////
///////////////////// Dice Tests ///////////////////////////
////////////////////////////////////////////////////////////
bool test_Dice() {
    return false;
}

bool test_Dice_roll() {
    bool success = true;

    Dice dice = Dice();
    for (int i = 1; i < 100; i++) {
        int result = dice.roll();
        success = success && (result < 7 && result > 0);
    }

    return success;
}

bool test_Dice_getHistory() {
    return false;
}


////////////////////////////////////////////////////////////
////////////////// DiceRoller Tests ////////////////////////
////////////////////////////////////////////////////////////
bool test_DiceRoller() {
    bool success = true;

    for (u_int i = 1; i < 4; i++) {
        try {
            DiceRoller roller = DiceRoller(i);
            success = success && roller.getNumDice() == i;
            success = success && roller.getDice().size() == i;
        }
        catch (int e) {
            success = false;
        }
    }

    return success;
}

bool test_DiceRoller_roll() {
    bool success = true;
    DiceRoller roller = DiceRoller(3);
    auto outcome = roller.roll();

    success = success && outcome.size() == 3;
    success = success && outcome.at(0) > 0 && outcome.at(0) < 7;
    success = success && outcome.at(1) > 0 && outcome.at(1) < 7;
    success = success && outcome.at(2) > 0 && outcome.at(2) < 7;

    return success;
}

bool test_DiceRoller_getHistory() {
  return false;
}

bool test_DiceRoller_getDice() {
    return false;
}

bool test_DiceRoller_getNumDice() {
    return false;
}

////////////////////////////////////////////////////////////
////////////////////// Run Tests ///////////////////////////
////////////////////////////////////////////////////////////
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
    assert(test_Dice());
    std::cout << "----- Testing Dice roll() method" << std::endl;
    assert(test_Dice_roll());
    std::cout << "----- Testing Dice getHistory()" << std::endl;
    assert(test_Dice_getHistory());

    std::cout << "" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "---------- Running tests for DiceRoller class ----------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "----- Testing DiceRoller constructor" << std::endl;
    assert(test_DiceRoller());
    std::cout << "----- Testing DiceRoller roll() method" << std::endl;
    assert(test_DiceRoller_roll());
    std::cout << "----- Testing DiceRoller getHistory() method" << std::endl;
    assert(test_DiceRoller_getHistory());

    std::cout << "----- Testing DiceRoller getDice() method" << std::endl;
    assert(test_DiceRoller());

    std::cout << "----- Testing DiceRoller getNumDice() method" << std::endl;
    assert(test_DiceRoller());

    return 0;
}
