#ifndef COMP_345_PROJ_REINFORCEMENTPHASE_H
#define COMP_345_PROJ_REINFORCEMENTPHASE_H

class ReinforcementPhase {
public:
    ReinforcementPhase(int player);
    int getNumberOfArmies();
    void placeArmies();

private:
    int* numberOfArmies;
    int* player;
    int countriesOwned();
    int continentControlValue();
    int cardExchange();
};

#endif //COMP_345_PROJ_REINFORCEMENTPHASE_H
