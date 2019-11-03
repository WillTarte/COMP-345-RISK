#include <iostream>

bool test_ReinforcementPhase() {
    bool pass = true;

    return pass;
}

bool test_getNumberOfArmies() {
    bool pass = true;

    return pass;
}

bool test_placeArmies() {
    bool pass = true;

    return pass;
}

////////////////////////////////////////////////////////////
////////////////////// Run Tests ///////////////////////////
////////////////////////////////////////////////////////////

#define assert(c, m, s) "Testing " << c << " " << m << "() method: " \
    << (s ? "\033[32mPass" : "\033[31mFail") << "\033[30m" << std::endl

int main() {
    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------ Running tests for ReinforcementPhase class ------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    std::cout << assert("ReinforcementPhase", "constructor", test_ReinforcementPhase());
    std::cout << assert("ReinforcementPhase", "getNumberOfArmies", test_getNumberOfArmies());
    std::cout << assert("ReinforcementPhase", "placeArmies", test_placeArmies());

    return 0;
}

