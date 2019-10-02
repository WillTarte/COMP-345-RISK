#include <string>
#include <vector>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class Dice {
public:
    Dice();
    int roll();
    std::vector<int> getHistory() { return *history; };

private:
    int* timesRolled;
    std::vector<int>* history;
    void saveRoll(int roll);
};

class DiceRoller {
public:
    DiceRoller(int _numDice);
    std::vector<int> roll();
    std::vector<std::vector<int>> getHistory();
    std::vector<Dice> getDice() { return *dice; };
    int getNumDice() { return *numDice; };

private:
    int* numDice;
    std::vector<Dice>* dice;
};

#endif // COMP_345_PROJ_DICE_H
