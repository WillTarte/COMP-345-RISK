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
        static void resetInstance();
        static GameLoop* getInstance();
        static void initInstance(Map* gameMap, vector<Player*>* playerList, Deck* deck);
        GameLoop(const GameLoop& toCopy) = delete;
        void operator=(GameLoop& rhs) = delete;
        void loop();

    void distributeArmies();

        bool isRoundFinished (unsigned long currentPlayerPosition);
        bool isGameDone (Player* currentPlayer);

    Map* getGameMap() { return gameMap; }
        inline vector<Player*>* getAllPlayers () { return allPlayers; }
        inline Deck* getGameDeck() { return gameDeck; }

private:
        static GameLoop* gameLoopInstance;
        Deck* gameDeck;
        Map* gameMap;
        vector<Player*>* allPlayers;
        GameLoop(Map* map, vector<Player*>* playerList, Deck* deck);
};

#endif //COMP_345_PROJ_GAMEENGINE_H
