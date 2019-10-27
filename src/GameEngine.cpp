#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err60-cpp"
//
// Created by claul on 10/27/2019.
//

#include "GameEngine.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
using namespace std;

namespace localConstants{
    const string yes = "yes";
    const string no = "no";
    const int fromReinforcement = 0;
    const int toReinforcement = 1;
}

/**
 * Game loop constructor
 * @param
 * @return
 */
GameLoop::GameLoop (const vector<Map::Country*> &countryList, const vector<Player> &playerList) {
    allCountries = countryList;
    allPlayers = playerList;
    gameNotDone = true;
}

/**
 * Loop for each round of the game. Checks if there is a winner at the end of each player's turn
 */
void GameLoop::playerTurn() {

    int currentPlayerPosition = 0;
    Player currentPlayer = getAllPlayers()[currentPlayerPosition];

    while(gameNotDone) {
        queryReinforcement(currentPlayer);

        currentPlayerPosition++;
        isRoundFinished(currentPlayerPosition);
    }

}

/**
 * Checks to see if the round is finished to go back to the first player
 * @param currentPlayerPosition
 * @return
 */
bool GameLoop::isRoundFinished(unsigned long currentPlayerPosition) {
    return currentPlayerPosition == GameLoop::getAllCountries().size();
}

void GameLoop::queryReinforcement(Player currentPlayer) {
    bool invalidDecision = true;
    string userDecision;
    Map::Country* fromCountry;
    Map::Country* toCountry;
    int numberOfArmies = 0;

    cout << "Do you want to reinforce a country? [yes / no]" << endl;

    while (invalidDecision) {
        cin >> userDecision;

        // put the input to lower case
        transform(userDecision.begin(), userDecision.end(), userDecision.begin(),
                  [](unsigned char c) { return std::tolower(c); });

        if (localConstants::yes == userDecision) {
            fromCountry = BasicMechanics::queryCountry(GameLoop::getAllCountries(), localConstants::fromReinforcement);
            toCountry = BasicMechanics::queryCountry(GameLoop::getAllCountries(), localConstants::toReinforcement);
            numberOfArmies = BasicMechanics::queryArmies();
            currentPlayer.reinforce(*fromCountry, *toCountry, numberOfArmies);
            invalidDecision = false;
        } else if (localConstants::no == userDecision) {
            invalidDecision = false;
        } else {
            cout << "Invalid decision. Do you want to reinforce a country? [yes / no]" << endl;
        }
    }
}

/**
 * Ask the user to input which country they want to interact with in either of the 3 phases
 * @param countryList
 * @param outputMessage
 * @return the country they want to interact with
 */
Map::Country* BasicMechanics::queryCountry(const vector<Map::Country*>& countryList, int outputMessage) {
    string country;
    int countryPosition;

    switch (outputMessage) {
        case localConstants::fromReinforcement:
            cout << "From which country do you want to dispatch the armies?" << endl;
            break;
        case localConstants::toReinforcement:
            cout << "To which country do you want to dispatch the armies?" << endl;
            break;
        default:
            throw invalid_argument("There is an unexpected bug with the system. System is terminating.");
    }

    cin >> country;
    countryPosition = BasicMechanics::findCountryPosition(countryList, country);

    while(countryPosition == -1) {
        cout << "Invalid input. Please try again" << endl;
        cin >> country;
        countryPosition = BasicMechanics::findCountryPosition(countryList, country);
    }

    return countryList[countryPosition];
}

/**
 * Takes the name of the country inputted by the user and finds it in the array of countries.
 * @param countryList
 * @return
 */
int BasicMechanics::findCountryPosition(vector<Map::Country*> countryList, const string& countryName) {
    for(int i = 0; i < static_cast<int>(countryList.size()); i++) {
        if (countryList[i]->getCountryName() == countryName) {
            return i;
        }
    }
    return -1;
}

/**
 * Ask the user for the number of armies they want to include in their current action
 * @return
 */
int BasicMechanics::queryArmies() {

    int numberOfArmies;

    cout << "How many armies would you like to send?" << endl;
    cin >> numberOfArmies;

    return numberOfArmies;
}

#pragma clang diagnostic pop