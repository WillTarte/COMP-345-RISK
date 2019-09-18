#include <string>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class Dice {
public:
  Dice();
  int roll();
  std::vector<int> getHistory();

private:
  std::vector<int>* history;
  void saveRoll();
};

class DiceRoller {
public:
  DiceRoller(int numDice);
  std::vector<int> roll();
  std::vector<int> getHistory();
  Dice[] getDice();
  int getNumDice();

private:
  int* numDice;
  Dice[]* dice;
};

#endif // COMP_345_PROJ_DICE_H
