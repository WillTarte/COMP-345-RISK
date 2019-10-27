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
    vector<Map::Country*> allCountries;
    vector<Player> allPlayers;
    bool gameNotDone;
    GameLoop (const vector<Map::Country*>&, const vector<Player>&);
public:
    void playerTurn();
    vector<Map::Country*> getAllCountries(){ return allCountries; }
    vector<Player> getAllPlayers() { return allPlayers; }
    bool isRoundFinished(unsigned long currentPlayerPosition);
    void queryReinforcement(Player currentPlayer);
};

namespace BasicMechanics{
    int findCountryPosition(vector<Map::Country*>, const string&);
    Map::Country* queryCountry(const vector<Map::Country*>& countryList, int);
    int queryArmies();
}
#endif //COMP_345_PROJ_GAMEENGINE_H
