//
// Created by claul on 10/27/2019.
//

#include "GameEngine.h"
#include "MapLoader.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <string>
using namespace std;

GameLoop* GameLoop::gameLoopInstance = nullptr;

/**
 * Game loop constructor
 * @param countryList - the list of all countries in the game
 * @return playerList - the list of all players in the game (where playerList[0] is the first player & playerList[n-1] is the last player
 */
GameLoop::GameLoop(vector<Map::Country *>* countryList, vector<Player*>* playerList) {
    allCountries = countryList;
    allPlayers = playerList;
}

void GameLoop::initInstance(vector<Map::Country*>* countryList, vector<Player*>* playerList) {
    if(GameLoop::gameLoopInstance == nullptr) {
        GameLoop::gameLoopInstance = new GameLoop(countryList, playerList);
    } else {
        std::cout << "Tried to create an instance of GameLoop, but one already exists!" << std::endl;
    }
}

GameLoop* GameLoop::getInstance() {
    if (gameLoopInstance == nullptr) {
        std::cout << "GameLoop instance was not initialized. Call GameLoop::initInstance first." << std::endl;
        return nullptr;
    } else {
        return gameLoopInstance;
    }
}

/**
 * Game loop destructor
 */
GameLoop::~GameLoop() {
    delete allCountries;
    delete allPlayers;
}

void GameLoop::resetInstance() {
    delete gameLoopInstance;
    gameLoopInstance = nullptr;
}

/**
 * Loop for each round of the game. Checks if there is a winner at the end of each player's turn
 */
void GameLoop::loop() {

    bool gameNotDone = true;
    int currentPlayerPosition = 0;
    Player* currentPlayer = allPlayers->at(currentPlayerPosition);

    while (gameNotDone) {
        cout << "\u001b[31m";  // for demo purposes
        cout << "Player " << allPlayers->at(currentPlayerPosition)->getPlayerId() << " is reinforcing!" << endl;
        cout << "\u001b[31m";
        currentPlayer->reinforce();

        cout << "\u001b[33m";  // for demo purposes
        cout << "Player " << allPlayers->at(currentPlayerPosition)->getPlayerId() << " is attacking!" << endl;
        cout << "\u001b[33m";
        currentPlayer->attack();

        cout << "\u001b[34m";  // for demo purposes
        cout << "Player " << allPlayers->at(currentPlayerPosition)->getPlayerId() << " is fortifying!" << endl;
        cout << "\u001b[34m";
        currentPlayer->fortify();

        gameNotDone = isGameDone(currentPlayer);

        if (gameNotDone) {
            currentPlayerPosition++;
            if (isRoundFinished(currentPlayerPosition)) {
                currentPlayerPosition = 0;
                currentPlayer->setOwnedCountries(allCountries); // for demo - give all countries to first player at the end of the round
            }
        }
    }
    cout << "\u001b[35m";
    cout << "Player " << allPlayers->at(currentPlayerPosition)->getPlayerId() << " owns all of the countries! They have won the game!!";
    cout << "\u001b[0m";
    cout.flush();
}

/**
 * Checks to see if the round is finished to go back to the first player
 * ie if there are 6 players, then the round restarts when currentPlayerPosition reaches 6
 * @param currentPlayerPosition
 * @return
 */
bool GameLoop::isRoundFinished(unsigned long currentPlayerPosition) {
    return currentPlayerPosition == allPlayers->size();
}

/**
 * Checks if a player owns all of the countries by checking if their vector of countries is the same size as the game's
 * @param currentPlayerPosition
 * @return
 */
bool GameLoop::isGameDone(Player* currentPlayer) {
    return currentPlayer->getOwnedCountries()->size() != allCountries->size();
}

/**
 * GameStarter constructor
 */
GameStarter::GameStarter(const vector<std::string>& fileNames) {
    gameMap = nullptr;
    gameDeck = nullptr;
    gamePlayers = nullptr;
    mapList = new vector<std::string*>();
    for(const auto& fileName : fileNames){
        mapList->push_back(new std::string(fileName));
    }
}

/**
 * GameStarter copy constructor
 * @param toCopy
 */
GameStarter::GameStarter(const GameStarter &toCopy) {
    gameMap = nullptr;
    gameDeck = nullptr;
    gamePlayers = nullptr;
    mapList = new vector<std::string*>();

    *gameMap = *toCopy.gameMap;
    *gameDeck = *toCopy.gameDeck;
    *gamePlayers = *toCopy.gamePlayers;
    *mapList = *toCopy.mapList;
}

/**
 * Gamestarter destructor
 */
 GameStarter::~GameStarter() {
     delete gameMap;
     delete gameDeck;
     delete gamePlayers;
     delete mapList;
 }

void GameStarter::operator=(GameStarter& rhs) {
    this->gameDeck = rhs.gameDeck;
    this->gameMap = rhs.gameMap;
    this->gamePlayers = rhs.gamePlayers;
    this->mapList = rhs.mapList;
}

/**
 * Main method of the starter
 */
void GameStarter::start() {
    auto* mapToLoad = new std::string(chooseMap());
    int numberOfPlayers = choosePlayerNumber();

    MapLoader myLoader = MapLoader(*mapToLoad);
    gameMap = myLoader.readMapFile();
    if(gameMap == nullptr || !gameMap->testConnected()){
        cout << "\nThere was an error loading the game board. Try another mapfile.\n";
        delete(mapToLoad);
        start();
        return;
    }
    gamePlayers = initPlayers(numberOfPlayers, gameMap);
    gameDeck = new Deck(gameMap->getMapCountries()->size());
    gameDeck->createDeck();
    gameMap->printMap();
    delete(mapToLoad);
}

/**
 * Prompts the user for a map
 * @return name of the map file
 */
std::string GameStarter::chooseMap() {
    if(!mapList->empty()){
        unsigned int mapChoice;
        unsigned int numberMaps;
        do{
            cout << "please choose a map :\n";
            numberMaps = 0;
            for(auto map : *mapList){
                numberMaps++;
                cout << numberMaps << " - " << *map << "\n";
            }
            cin >> mapChoice;
            cin.clear();
            cin.ignore(512, '\n');
        }while(mapChoice < 1 || mapChoice > numberMaps || isnan(mapChoice));
        return *(mapList->at(mapChoice - 1));
    }
    return "";
}

/**
 * Prompts the user for the number of players
 * @return the number of players
 */
int GameStarter::choosePlayerNumber() {
    unsigned int playerChoice;
    do{
        cout << "please choose a number of players between 2 and 6 :\n";
        cin >> playerChoice;
        cin.clear();
        cin.ignore(512, '\n');
    }while(playerChoice < 2 || playerChoice > 6 || isnan(playerChoice));
    return int(playerChoice);
}

/**
 * Creates the players and assigns countries to them
 * @param numPlayers the number of players
 * @param map the map object
 * @return the vector of players
 */
vector<Player*>* GameStarter::initPlayers(int numPlayers, Map* map){
    vector<vector<Map::Country*>> countriesPerPlayer;
    countriesPerPlayer.reserve(numPlayers);
    //split up the countries by the number of players
    for(int i = 0; i < numPlayers; i++){
        countriesPerPlayer.emplace_back();
    }
    //randomize map countries
    vector<Map::Country*>* randomizedCountries = map->getMapCountries();
    std::shuffle(randomizedCountries->begin(), randomizedCountries->end(), std::mt19937(std::random_device()()));
    for(unsigned int i = 0; i < randomizedCountries->size(); i++){
        Map::Country* currCountry = randomizedCountries->at(i);
        currCountry->setPlayerOwnerID(int(i)%numPlayers);
        countriesPerPlayer[i%numPlayers].push_back(currCountry);
    }

    auto* players = new vector<Player*>;
    players->reserve(numPlayers);
    //create the players with their respective list of countries created above
    for(int i = 0; i < numPlayers; i++){
        players->push_back(new Player(countriesPerPlayer[i], Hand(), DiceRoller(), i));
    }
    std::shuffle(players->begin(), players->end(), std::mt19937(std::random_device()()));
    return players;
}

/**
 * Prompt each player for army distribution
 * @param players the players in the game
 * @param offset the offset constant that randomizes the order of play
 */
void GameStarter::distributeArmies() {
    int numberOfPlayers = gamePlayers->size();
    int numberOfArmies = getNumberOfArmies(numberOfPlayers);
    int currentPlayerPosition = 0;
    Player* currentPlayer = gamePlayers->at(currentPlayerPosition);
    int counter = 0;
    cout << "\nEach player has " << numberOfArmies << " armies to place on their countries. \n";
    while (counter < numberOfPlayers) {
        cout << "Player " << currentPlayer->getPlayerId() << " , please place your armies. Here is your list of countries :\n";
        for(unsigned int i = 1; i <= currentPlayer->getOwnedCountries()->size(); i++){
            cout << i << " - " << currentPlayer->getOwnedCountries()->at(i-1)->getCountryName() << "\n";
        }
        for(int i = 1; i <= numberOfArmies; i++){
            int countryToPlaceOn;
            do{
                cout << "\nWhere do you place army number " << i;
                cin >> countryToPlaceOn;
                cin.clear();
                cin.ignore(512, '\n');
            }while(countryToPlaceOn < 1 || countryToPlaceOn > int(currentPlayer->getOwnedCountries()->size()) || isnan(countryToPlaceOn));
            //increment the number of troops on the selected country
            Map::Country* currCountry = currentPlayer->getOwnedCountries()->at(countryToPlaceOn-1);
            currCountry->setNumberOfTroops(currCountry->getNumberOfTroops()+1);
        }
        currentPlayerPosition++;
        currentPlayerPosition = currentPlayerPosition % numberOfPlayers;
        currentPlayer = gamePlayers->at(currentPlayerPosition);
        counter++;
    }
}

/**
 * Get the total number of armies for the game
 * @param numberOfPlayers the number of players
 * @return the number of armies per player in the game
 */
int GameStarter::getNumberOfArmies(int numberOfPlayers) {
   switch(numberOfPlayers){
       case 2: return 40;
       case 3: return 35;
       case 4: return 30;
       case 5: return 25;
       case 6: return 20;
       default: return 999;
   }
}
