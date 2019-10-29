//
// Created by claul on 10/27/2019.
//

#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Dice.h"
#include "Player.h"

using namespace std;

#ifndef COMP_345_PROJ_GAMEENGINE_H
#define COMP_345_PROJ_GAMEENGINE_H

class GameLoop {
    vector<Map::Country *> allCountries;
    vector<Player> allPlayers;

public:
    GameLoop(const vector<Map::Country*> &, const vector<Player> &);
    void loop();
    vector<Player> getAllPlayers () { return allPlayers; }
    static bool isRoundFinished (unsigned long currentPlayerPosition, const vector<Player> &playerList);
    static bool isGameDone (Player currentPlayer, const vector<Map::Country *> &countryList);
};

#endif //COMP_345_PROJ_GAMEENGINE_H
