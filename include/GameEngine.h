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
    GameStarter(const GameStarter& toCopy);
    void operator=(GameStarter& rhs);
    void start();
    Map* getGameMap(){return gameMap;};
    vector<Player*>* getGamePlayers(){return gamePlayers;};
    Deck* getGameDeck(){return gameDeck;};
    void distributeArmies();
private:
    vector<string*>* mapList;
    string chooseMap();
    static int choosePlayerNumber();
    static vector<Player*>* initPlayers(int numPlayers, Map* map);
    static int getNumberOfArmies(int numberOfPlayers);
    vector<Player*>* gamePlayers;
    Map* gameMap;
    Deck* gameDeck;
};


class GameLoop {
    public:
        ~GameLoop();
        static void start();
        static void resetInstance();
        static GameLoop* getInstance();
        static void initInstance(Map* gameMap, vector<Player*>* playerList, Deck* deck);
        GameLoop(const GameLoop& toCopy) = delete;
        void operator=(GameLoop& rhs) = delete;
        void loop();
        Map* getGameMap() { return gameMap; }
        bool isRoundFinished (unsigned long currentPlayerPosition);
        bool isGameDone (Player* currentPlayer);
        inline vector<Map::Country*>* getAllCountries () { return allCountries; }
        inline vector<Player*>* getAllPlayers () { return allPlayers; }
        inline Deck* getGameDeck() { return gameDeck; }

private:
        static GameLoop* gameLoopInstance;
        Deck* gameDeck;
        vector<Map::Country *>* allCountries;
        Map* gameMap;
        vector<Player*>* allPlayers;
        GameLoop(Map* map, vector<Player*>* playerList, Deck* deck);
};

#endif //COMP_345_PROJ_GAMEENGINE_H
