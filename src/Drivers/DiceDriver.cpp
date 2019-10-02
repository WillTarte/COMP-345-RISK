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

////////////////////////////////////////////////////////////
////////////////// DiceRoller Tests ////////////////////////
////////////////////////////////////////////////////////////
bool test_DiceRoller() {
    bool success = true;

    for (int i = 1; i < 4; i++) {
        try {
            auto* roller = new DiceRoller();
            success = success && roller->getDiceRolled() == 0;

            for (int x = 0 ; x < (long)roller->getHistory().size() ; x++) {
                success = success && roller->getHistory().at(x) == 0;
            }
        }
        catch (int e) {
            success = false;
        }
    }

    return success;
}

bool test_DiceRoller_roll() {
    bool success = true;
    auto* roller = new DiceRoller();
    std::vector<int> outcome = roller->roll(3);

    success = success && outcome.size() == 3;
    success = success && outcome[0] > 0 && outcome[0] < 7;
    success = success && outcome[1] > 0 && outcome[1] < 7;
    success = success && outcome[2] > 0 && outcome[2] < 7;
    success = success && outcome[0] <= outcome[1] && outcome[1] <= outcome[2];

    return success;
}

bool test_DiceRoller_getHistory() {
    bool success = true;
    auto* roller = new DiceRoller();

    std::vector<int> outcomes = {0, 0, 0, 0, 0, 0};
    for (int x = 0 ; x < 10 ; x++) {
        std::vector<int> results = roller->roll(3);
        for (int y = 0 ; y < (long)results.size() ; y++) {
            outcomes[results[y]-1]++;
        }
    }

    auto history = roller->getHistory();
    for (long x = 0; x < (long)outcomes.size() ; x++)
        success = success && outcomes[x] == history[x];

    return success;
}

bool test_DiceRoller_getDiceRolled() {
    auto* roller = new DiceRoller();

    int rolls = 0;
    for (int x = 0 ; x < 10 ; x++) {
        rolls += x;
        roller->roll(x);
    }

    return roller->getDiceRolled() == rolls;
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

    std::cout << "\033[34m" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "---------- Running tests for DiceRoller class ----------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    std::cout << "Testing DiceRoller constructor: " << assert(test_DiceRoller());
    std::cout << "Testing DiceRoller roll() method: " << assert(test_DiceRoller_roll());
    std::cout << "Testing DiceRoller getHistory() method: " << assert(test_DiceRoller_getHistory());
    std::cout << "Testing DiceRoller getNumDice() method: " << assert(test_DiceRoller_getDiceRolled());

    return 0;
}
