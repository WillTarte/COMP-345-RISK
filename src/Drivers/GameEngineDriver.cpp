//
// Created by claul on 10/27/2019.
//

#include "../../include/Map.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"
#include <iostream>

using std::cout;
using std::vector;

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Main method ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

int main() {

    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------ Running the game start phase --------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    cout << "\n";

    GameLoop::start();

    for (unsigned long i = 0; i < GameLoop::getInstance()->getAllPlayers()->size(); i++) {
        for (unsigned long j = 0;
             j < GameLoop::getInstance()->getAllPlayers()->at(i)->getOwnedCountries()->size(); j++) {
            cout << "player " << i << " owns country : "
                 << GameLoop::getInstance()->getAllPlayers()->at(i)->getOwnedCountries()->at(j)->getCountryName()
                 << "\n";
        }
    }

    cout << "\nprint map to compare with output of start : \n";

    GameLoop::getInstance()->getGameMap()->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "-------------------------- Deck ------------------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    cout << "Number of cards in the deck: " << GameLoop::getInstance()->getGameDeck()->getNumberOfCards() << endl;
    cout << "Number of countries in the map: " << GameLoop::getInstance()->getGameMap()->getMapCountries()->size()
         << endl;

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------------ Distributing armies -----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    GameLoop::getInstance()->distributeArmies();

    cout << "\nprint map to see where armies went : \n";

    GameLoop::getInstance()->getGameMap()->printMap();

    std::cout << "\n\n\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "--------------- Running the main game loop -------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    GameLoop::getInstance()->loop();
    GameLoop::resetInstance();
    return 0;
}