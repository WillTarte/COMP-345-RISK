#include <string>
#include <vector>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class Dice {
public:
  Dice();
  int roll();
  std::vector<int> getHistory();

private:
  std::vector<int>* history;
  void saveRoll(int roll);
};

class DiceRoller {
public:
  DiceRoller(int numDice);
  int* roll();
  std::vector<int>* getHistory();
  Dice* getDice();
  int getNumDice();

private:
  int* numDice;
  Dice* dice[];
};

#endif // COMP_345_PROJ_DICE_H
