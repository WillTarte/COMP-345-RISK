#include "../include/PlayerStrategy.h"

char HumanPlayerStrategy::yesOrNo(StrategyContext _) {
  char playerDecision = 0;
  std::cin >> playerDecision;

  return playerDecision;
}

int HumanPlayerStrategy::intInput(StrategyContext _) {
  int count = 0;
  std::cin >> count;

  return count;
}

char AggressiveBotStrategy::yesOrNo(StrategyContext context) { return 0; }
int AggressiveBotStrategy::intInput(StrategyContext context) { return 0; }

char BenevolentBotStrategy::yesOrNo(StrategyContext context) { return 0; }
int BenevolentBotStrategy::intInput(StrategyContext context) { return 0; }
