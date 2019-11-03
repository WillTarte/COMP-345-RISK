#include "ReinforcementPhase.h"
#include "GameEngine.h"
#include "Map.h"
#include <vector>

ReinforcementPhase::ReinforcementPhase(Player player, Map map) {
    this->player = &player;
    this->map = &map;
    this->numberOfArmies = 0;
}

int ReinforcementPhase::getNumberOfArmies() {
    auto countries = countriesOwned();
    numberOfArmies += countries;

    auto controlValue = continentControlValue();
    numberOfArmies += controlValue;

    auto exchange = cardExchange();
    numberOfArmies += exchange;

    return *numberOfArmies;
}

void ReinforcementPhase::placeArmies() {}

int ReinforcementPhase::countriesOwned() {
    auto countries = 0;

    for (auto* country : *map->getMapCountries()) {
        if (country->getPlayerOwnerID() == player->getPlayerId()) {
            countries++;
        }
    }

    if (countries < 9) {
        return 3;
    }

    return countries / 3;
}

int ReinforcementPhase::continentControlValue() {
    auto value = 0;
    for (Map::Continent* cont : *map->getMapContinents()) {
        bool fullControl = true;

        // Check if player controls all countries in continent
        for (Map::Country* country : *cont->getCountriesInContinent()) {
            if (player->getPlayerId() != country->getPlayerOwnerID()) {
                fullControl = false;
                break;
            }
        }

        if (fullControl) {
            // Continent control values come from:
            // https://www.ultraboardgames.com/risk/game-rules.php
            auto name = cont->getContinentName();
            if (name == "Asia")
                value += 7;
            if (name == "North America")
                value += 5;
            if (name == "Europe")
                value += 5;
            if (name == "Africa")
                value += 3;
            if (name == "South America")
                value += 2;
            if (name == "Australia")
                value += 2;
        }
    }

    return value;
}

int ReinforcementPhase::cardExchange() {
    auto exchange = 0;

    return exchange;
}
