#include "ReinforcementPhase.h"

ReinforcementPhase::ReinforcementPhase(int player) :
        player(new int(player)), numberOfArmies(0) {}

int ReinforcementPhase::getNumberOfArmies() {
    auto countries = countriesOwned();
    numberOfArmies += countries < 3 ? 3 : countries / 3;

    auto controlValue = continentControlValue();
    numberOfArmies += controlValue;

    auto exchange = cardExchange();
    numberOfArmies += exchange;

    return *numberOfArmies;
}

void ReinforcementPhase::placeArmies() {}

int ReinforcementPhase::countriesOwned() {
    auto countries = 0;

    return countries;
}

int ReinforcementPhase::continentControlValue() {
    auto value = 0;

    return value;
}

int ReinforcementPhase::cardExchange() {
    auto exchange = 0;

    return exchange;
}
