#include <iostream>
#include "../../include/Dice.h"

////////////////////////////////////////////////////////////
///////////////////// Dice Tests ///////////////////////////
////////////////////////////////////////////////////////////
bool test_Dice() {
    try {
        Dice* dice = new Dice();
        delete (dice);
        return true;
    }
    catch (int e) {
        return false;
    }
}

bool test_Dice_roll() {
    bool success = true;

    for (int i = 1; i < 100; i++) {
        int result = Dice::roll();
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

            for (int x = 0; x < (long) roller->getHistory().size(); x++) {
                success = success && roller->getHistory().at(x) == 0;
            }

            delete (roller);
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

    for (int x = 1; x < 4; x++) {
        std::vector<int> outcome = roller->roll(x);

        success = success && x == (int) outcome.size();
        int prev = outcome[0];
        for (int y = 0; y < x; y++) {
            success = success && outcome[y] > 0 && outcome[y] < 7;
            success = success && prev <= outcome[y];
        }
    }

    delete (roller);

    return success;
}

bool test_DiceRoller_getHistory() {
    bool success = true;
    auto* roller = new DiceRoller();

    std::vector<int> outcomes = {0, 0, 0, 0, 0, 0};
    for (int x = 0; x < 10; x++) {
        std::vector<int> results = roller->roll(3);
        for (int result : results) {
            outcomes[result - 1]++;
        }
    }

    auto history = roller->getHistory();
    for (long x = 0; x < (long) outcomes.size(); x++)
        success = success && outcomes[x] == history[x];

    delete (roller);

    return success;
}

bool test_DiceRoller_getDiceRolled() {
    auto* roller = new DiceRoller();

    int rolls = 0;
    for (int x = 0; x < 10; x++) {
        rolls += x;
        roller->roll(x);
    }

    bool success = roller->getDiceRolled() == rolls;
    delete (roller);

    return success;
}

bool test_DiceRoller_getPercentages() {
    bool success = true;
    int timesRolled = 0;
    auto* history = new std::vector<int>(6);
    auto* roller = new DiceRoller();
    for (int x = 0; x < 100; x++) {
        int roll = roller->roll(1).at(0);
        timesRolled++;
        (*history)[roll - 1]++;
    }

    for (int x = 0; x < (int) history->size(); x++) {
        double percent = (double) history->at(x) / (double) timesRolled;
        success = success && percent == roller->getPercentages()[x];
    }

    delete (history);
    delete (roller);

    return success;
}

////////////////////////////////////////////////////////////
////////////////////// Run Tests ///////////////////////////
////////////////////////////////////////////////////////////

#define assert(c, m, s) "Testing " << c << " " << m << "() method: " \
    << (s ? "\033[32mPass" : "\033[31mFail") << "\033[30m" << std::endl

int main() {
    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------- Running tests for Dice class -------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    std::cout << assert("Dice", "constructor", test_Dice());
    std::cout << assert("Dice", "roll", test_Dice_roll());

    std::cout << "\033[34m" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "---------- Running tests for DiceRoller class ----------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    std::cout << assert("DiceRoller", "constructor", test_DiceRoller());
    std::cout << assert("DiceRoller", "roll", test_DiceRoller_roll());
    std::cout << assert("DiceRoller", "getHistory", test_DiceRoller_getHistory());
    std::cout << assert("DiceRoller", "getDiceRoller", test_DiceRoller_getDiceRolled());
    std::cout << assert("DiceRoller", "getPercentages", test_DiceRoller_getPercentages());

    return 0;
}
