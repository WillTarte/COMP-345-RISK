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
    std::cout << "---------------- Running the game starter --------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    cout << "\n";

    GameStarter starter = GameStarter({"world.map","world2.map","worldEmpty.map", "otherWorkingMap.map"});
    starter.start();

    for(unsigned long i = 0; i < starter.getGamePlayers()->size(); i++){
        for(unsigned long j = 0 ; j <starter.getGamePlayers()->at(i)->getOwnedCountries()->size(); j++){
            cout << "player " << i << " owns country : " << starter.getGamePlayers()->at(i)->getOwnedCountries()->at(j)->getCountryName() << "\n";
        }
    }

    cout << "\nprint map to compare with output of game starter : \n";

    starter.getGameMap()->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "-------------------------- Deck ------------------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    cout << "Number of cards in the deck: " << *starter.getGameDeck()->getNumberOfCards() << endl;
    cout << "Number of countries in the map: " << starter.getGameMap()->getMapCountries()->size() << endl;

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------------ Distributing armies -----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    starter.distributeArmies();

    cout << "\nprint map to see where armies went : \n";

    starter.getGameMap()->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "--------------- Running the main game loop -------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    auto* map = new Map(*starter.getGameMap());
    auto* playerList = new vector<Player*>(*starter.getGamePlayers());
    GameLoop::initInstance(map, playerList);
    GameLoop::getInstance()->loop();
    GameLoop::resetInstance();

    return 0;
}