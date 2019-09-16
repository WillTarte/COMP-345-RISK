#include <string>

#ifndef COMP_345_PROJ_DICE_H
#define COMP_345_PROJ_DICE_H

class Dice {
 public:
  Dice();
  void rollDice();

 private:
  int* sides;
}

#endif // COMP_345_PROJ_DICE_H
