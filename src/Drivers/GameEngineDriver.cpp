//
// Created by claul on 10/27/2019.
//

#include "../../include/Map.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"
#include <vector>
#include <iostream>
#include <random>

using std::cout;
using std::vector;

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Main method ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

int main() {

    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "----------------- Running the game starter ----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    cout << "\n";

    GameStarter starter = GameStarter({"world.map","world2.map","worldEmpty.map"});
    starter.start();

    vector<Player*>* players = starter.getGamePlayers();
    Map* map = starter.getGameMap();

    for(unsigned int i=0;i<players->size();i++){
        for(unsigned int j=0;j<players->at(i)->getOwnedCountries()->size();j++){
            cout << "player " << i << " owns country : " << players->at(i)->getOwnedCountries()->at(j)->getCountryName() << "\n";
        }
    }

    cout << "\nprint map to compare with output of game starter : \n";

    map->printMap();

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> startPos(0,players->size()-1);
    const int STARTING_POS = startPos(rng);

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "----------------- Distributing armies ----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    GameStarter::distributeArmies(players,STARTING_POS);

    cout << "\nprint map to see where armies went : \n";

    map->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "----------------- Running the main game loop ----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";
    GameLoop gameLoop = GameLoop(map->getMapCountries(), players);
    gameLoop.loop(players,STARTING_POS);

}