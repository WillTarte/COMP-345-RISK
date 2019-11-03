#include "Map.h"
#include "Player.h"
#include <vector>

#ifndef COMP_345_PROJ_REINFORCEMENTPHASE_H
#define COMP_345_PROJ_REINFORCEMENTPHASE_H

class ReinforcementPhase {
public:
    ReinforcementPhase(Player player, Map map);
    int getNumberOfArmies();
    void placeArmies();

private:
    int* numberOfArmies;
    Player* player;
    Map* map;
    int countriesOwned();
    int continentControlValue();
    int cardExchange();
};

#endif //COMP_345_PROJ_REINFORCEMENTPHASE_H
