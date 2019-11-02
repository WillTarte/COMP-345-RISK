//
// Created by claul on 10/27/2019.
//

#include "../../include/Map.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"
#include <vector>
#include <iostream>

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
    Deck* deck = starter.getGameDeck();

    for(unsigned int i = 0; i < players->size(); i++){
        for(unsigned int j = 0 ; j < players->at(i)->getOwnedCountries()->size(); j++){
            cout << "player " << i << " owns country : " << players->at(i)->getOwnedCountries()->at(j)->getCountryName() << "\n";
        }
    }

    cout << "\nprint map to compare with output of game starter : \n";

    map->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "----------------- Distributing armies ----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    starter.distributeArmies();

    cout << "\nprint map to see where armies went : \n";

    map->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "----------------- Running the main game loop ----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";
    GameLoop gameLoop = GameLoop(map->getMapCountries(), players);
    gameLoop.loop();

    delete(players);
    delete(map);
    delete(deck);

}