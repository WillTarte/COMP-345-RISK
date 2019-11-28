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
    public:
        ~GameLoop();
        static void start();
        static void startTournament();
        static void startSingle();
        static void resetInstance();
        static GameLoop* getInstance();
        static void initInstance(Map* gameMap, vector<Player*>* playerList, Deck* deck);
        GameLoop(const GameLoop& toCopy) = delete;
        void operator=(GameLoop& rhs) = delete;
        void loop();

    void distributeArmies();

        bool isRoundFinished (unsigned long currentPlayerPosition);
        static bool isGameDone (Player* currentPlayer,Map* currMap);
        Map* getGameMap() { return gameMaps->at(0); }
        Map* getCurrentMap(){return gameMaps->at(*currentMap);}
        vector<Map*>* getGameMaps() {return gameMaps;}
        void addGameMap(Map* map) { gameMaps->push_back(map); }
        inline vector<Player*>* getAllPlayers () { return allPlayers; }
        inline Deck* getGameDeck() { return gameDeck; }

private:
        static GameLoop* gameLoopInstance;
        int* currentMap;
        Deck* gameDeck;
        vector<Map*>* gameMaps;
        vector<Player*>* allPlayers;
        GameLoop(Map* map, vector<Player*>* playerList, Deck* deck);
};

#endif //COMP_345_PROJ_GAMEENGINE_H
