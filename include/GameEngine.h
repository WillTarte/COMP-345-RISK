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

class GameStarter {
public:
    explicit GameStarter(const vector<string>& fileNames);
    ~GameStarter();
    void start();
    Map* getGameMap(){return gameMap;};
    vector<Player*>* getGamePlayers(){return gamePlayers;};
    Deck* getGameDeck(){return gameDeck;};
    void distributeArmies();
private:
    vector<string*>* mapList;
    string chooseMap();
    static int choosePlayerNumber();
    static vector<Player*>* initPlayers(int numPlayers, Map map);
    static int getNumberOfArmies(int numberOfPlayers);
    vector<Player*>* gamePlayers;
    Map* gameMap;
    Deck* gameDeck;
};


class GameLoop {
    vector<Map::Country *>* allCountries;
    vector<Player*>* allPlayers;

public:
    GameLoop(vector<Map::Country*>* countryList, vector<Player*>* playerList);
    ~GameLoop();
    void loop();
    vector<Player*> getAllPlayers () { return *allPlayers; }
    bool isRoundFinished (unsigned long currentPlayerPosition);
    bool isGameDone (Player currentPlayer);
};

#endif //COMP_345_PROJ_GAMEENGINE_H
