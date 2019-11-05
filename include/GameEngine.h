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
        static void resetInstance();
        GameLoop(const GameLoop& toCopy) = delete;
        void operator=(GameLoop& rhs) = delete;
        static void initInstance(Map* gameMap, vector<Player*>* playerList);
        static GameLoop* getInstance();
        void loop();
        vector<Player*> getAllPlayers () { return *allPlayers; }
        Map* getGameMap() { return gameMap; }
        bool isRoundFinished (unsigned long currentPlayerPosition);
        bool isGameDone (Player* currentPlayer);
        bool isRoundFinished (unsigned long currentPlayerPosition);
        inline vector<Map::Country*> getAllCountries () { return *allCountries; }
        inline vector<Player*> getAllPlayers () { return *allPlayers; }
        inline static Deck getGameDeck() { return *gameDeck; }
        inline static void initGameDeck(Deck* deck) { gameDeck = deck; }

private:
        static GameLoop* gameLoopInstance;
        static Deck* gameDeck;
        vector<Map::Country *>* allCountries;
        Map* gameMap;
        vector<Player*>* allPlayers;
        GameLoop(Map* map, vector<Player*>* playerList);
};

#endif //COMP_345_PROJ_GAMEENGINE_H
