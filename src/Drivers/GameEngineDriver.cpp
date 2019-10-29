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
//////////////////////////// Game Loop Construction /////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

vector<Map::Country*> listOfCountries() {
    auto* france = new Map::Country(0, "france", 0);
    auto* germany = new Map::Country(1, "germany", 0);
    auto* quebec = new Map::Country(2, "quebec", 0);

    return {france, germany, quebec};
}

vector<Player> listOfPlayers(const vector<Map::Country *> &countryList) {
    vector<Map::Country*> ownedCountries0 = {countryList[0]};
    vector<Map::Country*> ownedCountries1 = {countryList[1]};
    vector<Map::Country*> ownedCountries2 = {countryList[2]};

    Hand emptyDeck = Hand();
    DiceRoller emptyDice = DiceRoller();

    Player player0 = Player(ownedCountries0, emptyDeck, emptyDice, 0);
    Player player1 = Player(ownedCountries1, emptyDeck, emptyDice, 1);
    Player player2 = Player(ownedCountries2, emptyDeck, emptyDice, 2);

    return {player0, player1, player2};
}

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Main method ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

int main() {
    vector<Map::Country *> countryList = listOfCountries();
    vector<Player> playerList = listOfPlayers(countryList);
    GameLoop gameLoop = GameLoop(countryList, playerList);

    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "----------------- Running the game loop ----------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    gameLoop.loop();
}