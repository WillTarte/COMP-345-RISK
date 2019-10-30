//
// Created by claul on 10/27/2019.
//

#include "GameEngine.h"
#include <vector>

using namespace std;

/**
 * Game loop constructor
 * @param countryList - the list of all countries in the game
 * @return playerList - the list of all players in the game (where playerList[0] is the first player & playerList[n-1] is the last player
 */
GameLoop::GameLoop(vector<Map::Country *> countryList, vector<Player> playerList) {
    allCountries = &countryList;
    allPlayers = &playerList;
}

/**
 * Loop for each round of the game. Checks if there is a winner at the end of each player's turn
 */
void GameLoop::loop() {

    bool gameNotDone = true;
    int currentPlayerPosition = 0;
    Player currentPlayer = getAllPlayers()[currentPlayerPosition];

    while (gameNotDone) {
        cout << "\u001b[31m";  // for demo purposes
        cout << "Player " << currentPlayerPosition << " is reinforcing!" << endl;
        cout << "\u001b[31m";
        currentPlayer.reinforce();

        cout << "\u001b[33m";  // for demo purposes
        cout << "Player " << currentPlayerPosition << " is attacking!" << endl;
        cout << "\u001b[33m";
        currentPlayer.attack();

        cout << "\u001b[34m";  // for demo purposes
        cout << "Player " << currentPlayerPosition << " is fortifying!" << endl;
        cout << "\u001b[34m";
        currentPlayer.fortify();

        gameNotDone = isGameDone(currentPlayer, *allCountries);

        if (gameNotDone) {
            currentPlayerPosition++;
            if (isRoundFinished(currentPlayerPosition, *allPlayers)) {
                currentPlayerPosition = 0;
                currentPlayer.getOwnedCountries() = *allCountries; // for demo - give all countries to player 0 at the end of the round
            }
        }
    }
    cout << "\u001b[35m";
    cout << "Player " << currentPlayerPosition << " owns all of the countries! They have won the game!!";
    cout << "\u001b[0m";
}

/**
 * Checks to see if the round is finished to go back to the first player
 * ie if there are 6 players, then the round restarts when currentPlayerPosition reaches 6
 * @param currentPlayerPosition
 * @return
 */
bool GameLoop::isRoundFinished(unsigned long currentPlayerPosition, const vector<Player> &playerList) {
    return currentPlayerPosition == playerList.size();
}

/**
 * Checks if a player owns all of the countries by checking if their vector of countries is the same size as the game's
 * @param currentPlayerPosition
 * @return
 */
bool GameLoop::isGameDone(Player currentPlayer, const vector<Map::Country *> &countryList) {
    return currentPlayer.getOwnedCountries().size() != countryList.size();
}