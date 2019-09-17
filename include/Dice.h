#include <string>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class DiceRoller {
public:
  DiceRoller(int numDice);
  std::vector<int> roll();
  std::vector<int> getRollHistory();

private:
  int* numDice;
  std::vector<Dice>* dice;
};

class Dice {
public:
  int roll();

private:
  std::vector<int>* pastRolls;
  void saveRoll();
}

#endif // COMP_345_PROJ_DICE_H
