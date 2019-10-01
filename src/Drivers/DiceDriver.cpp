#include <iostream>
#include "../../include/Dice.h"
#define assert(success) (success ? "\033[32mPass" : "\033[31mFail") << "\033[30m" << std::endl

////////////////////////////////////////////////////////////
///////////////////// Dice Tests ///////////////////////////
////////////////////////////////////////////////////////////
bool test_Dice() {
    try {
        Dice *dice = new Dice();
        delete(dice);
        return true;
    }
    catch (int e) {
        return false;

    }
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
    bool success = true;

    Dice* d = new Dice();
    std::vector<u_int> history = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 100; i++) {
        int outcome = d->roll();
        history[outcome-1]++;
        success = success && history[outcome-1] == d->getHistory()[outcome-1];
    }

    return success;
}


////////////////////////////////////////////////////////////
////////////////// DiceRoller Tests ////////////////////////
////////////////////////////////////////////////////////////
bool test_DiceRoller() {
    bool success = true;

    for (u_int i = 1; i < 4; i++) {
        try {
            auto* roller = new DiceRoller(i);
            success = success && roller->getDice().size() == i;
        }
        catch (int e) {
            success = false;
        }
    }

    return success;
}

bool test_DiceRoller_roll() {
    bool success = true;
    auto* roller = new DiceRoller(3);
    std::vector<u_int> outcome = roller->roll();

    success = success && outcome.size() == 3;
    success = success && outcome[0] > 0 && outcome[0] < 7;
    success = success && outcome[1] > 0 && outcome[1] < 7;
    success = success && outcome[2] > 0 && outcome[2] < 7;
    success = success && outcome[0] <= outcome[1] && outcome[1] <= outcome[2];

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
int main() {
    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------- Running tests for Dice class -------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    std::cout << "Testing Dice constructor: " << assert(test_Dice());
    std::cout << "Testing Dice roll() method: " << assert(test_Dice_roll());
    std::cout << "Testing Dice getHistory() method: " << assert(test_Dice_getHistory());

    std::cout << "\033[34m" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "---------- Running tests for DiceRoller class ----------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    std::cout << "Testing DiceRoller constructor: " << assert(test_DiceRoller());
    std::cout << "Testing DiceRoller roll() method: " << assert(test_DiceRoller_roll());
    std::cout << "Testing DiceRoller getHistory() method: " << assert(test_DiceRoller_getHistory());
    std::cout << "Testing DiceRoller getDice() method: " << assert(test_DiceRoller_getDice());
    std::cout << "Testing DiceRoller getNumDice() method: " << assert(test_DiceRoller_getNumDice());

    return 0;
}
