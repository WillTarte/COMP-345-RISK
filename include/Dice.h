#include <string>
#include <vector>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class Dice {
public:
    static int roll();
};

class DiceRoller {
public:
    DiceRoller();

    std::vector<int> roll(int numDice);

    std::vector<double> getPercentages();

    int getDiceRolled() { return *diceRolled; }

    std::vector<int> getHistory() { return *history; }

private:
    int* diceRolled;
    std::vector<int>* history;

    void saveRoll(int roll);
};

#endif // COMP_345_PROJ_DICE_H
