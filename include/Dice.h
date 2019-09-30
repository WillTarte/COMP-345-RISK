#include <string>
#include <vector>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class Dice {
public:
    Dice();
    u_int roll();
    std::vector<u_int> getHistory();

private:
    u_int* timesRolled;
    std::vector<u_int>* history;
    void saveRoll(u_int roll);
};

class DiceRoller {
public:
    DiceRoller(u_int numDice);
    std::vector<u_int> roll();
    std::vector<std::vector<u_int>> getHistory();
    std::vector<Dice> getDice();
    u_int getNumDice();

private:
    u_int* numDice{};
    std::vector<Dice>* dice;
};

#endif // COMP_345_PROJ_DICE_H
