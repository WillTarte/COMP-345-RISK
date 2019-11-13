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
#include <experimental/filesystem>
#include <GameObservers.h>

using namespace std;
namespace fs = std::experimental::filesystem;

// Before the singleton is initialized
GameLoop* GameLoop::gameLoopInstance = nullptr;

/**
 * Game loop constructor
 * @param countryList - the list of all countries in the game
 * @return playerList - the list of all players in the game (where playerList[0] is the first player & playerList[n-1] is the last player
 */
GameLoop::GameLoop(Map* map, vector<Player*>* playerList, Deck* deck) {
    gameMap = map;
    allPlayers = playerList;
    gameDeck = deck;
}

/**
 * Singleton initializer for GameLoop instance. The reason why there is both initInstance and getInstance is because
 * some data is resolved at runtime.
 *
 * @param map the map loaded through MapLoader
 * @param playerList the list of initialized players
 * @param deck a fully initialized deck
 */
void GameLoop::initInstance(Map* map, vector<Player*>* playerList, Deck* deck) {
    if (GameLoop::gameLoopInstance == nullptr) {
        GameLoop::gameLoopInstance = new GameLoop(map, playerList, deck);
    } else {
        std::cout << "Tried to create an instance of GameLoop, but one already exists!" << std::endl;
    }
}

/**
 * Getter for the instance of GameLoop, if it has been initialized through GameLoop::initInstance(args)
 *
 * @return the instance of GameLoop, or nullptr if it does not yet exist
 */
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
    delete gameMap;
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
    Player* currentPlayer = nullptr;

    while (gameNotDone) {

        // TODO: "This (phase information) should dynamically be updated as the game goes through different players/phases and be visible at all times during game play"
        currentPlayer = allPlayers->at(currentPlayerPosition);

        cout << "\u001b[31m";  // for demo purposes
        cout << "Player " << currentPlayer->getPlayerId() << " is reinforcing!" << endl;
        cout << "\u001b[31m";
        currentPlayer->reinforce();

        cout << "\u001b[33m";  // for demo purposes
        cout << "Player " << currentPlayer->getPlayerId() << " is attacking!" << endl;
        cout << "\u001b[33m";
        currentPlayer->attack();

        cout << "\u001b[34m";  // for demo purposes
        cout << "Player " << currentPlayer->getPlayerId() << " is fortifying!" << endl;
        cout << "\u001b[34m";
        currentPlayer->fortify();

        gameNotDone = !isGameDone(currentPlayer);

        if (gameNotDone) {
            currentPlayerPosition++;
            if (isRoundFinished(currentPlayerPosition)) {
                currentPlayerPosition = 0;
                // for demo - give all countries to first player at the end of the round
                currentPlayer->setOwnedCountries(gameMap->getMapCountries());
            }
        }
    }
    cout << "\u001b[35m";
    cout << "Player " << currentPlayer->getPlayerId() << " owns all of the countries! They have won the game!!";
    cout << "\u001b[0m";
    cout.flush();
}

/**
 * Checks to see if the round is finished to go back to the first player
 * ie if there are 6 players, then the round restarts when currentPlayerPosition reaches 6
 * @param currentPlayerPosition
 * @return true, if the round is finished (i.e. every player has had their turn)
 */
bool GameLoop::isRoundFinished(unsigned long currentPlayerPosition) {
    return currentPlayerPosition == allPlayers->size();
}

/**
 * Checks if a player owns all of the countries by checking if their vector of countries is the same size as the game's
 * @param currentPlayerPosition
 * @return true, if the currently playing player owns all the countries
 */
bool GameLoop::isGameDone(Player* currentPlayer) {
    return currentPlayer->getOwnedCountries()->size() == gameMap->getMapCountries()->size();
}

/**
 * Prompts the user for a map
 * @return name of the map file
 */
static std::string chooseMap() {
    vector<std::string> mapList = vector<std::string>();
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().has_extension() && entry.path().extension().string() == ".map") {
            mapList.push_back(entry.path().filename());
        }
    }
    if (!mapList.empty()) {
        unsigned int mapChoice;
        unsigned int numberMaps;
        do {
            cout << "\nplease choose a map :\n";
            numberMaps = 0;
            for (const auto& map : mapList) {
                numberMaps++;
                cout << numberMaps << " - " << map << "\n";
            }
            cin >> mapChoice;
            cin.clear();
            cin.ignore(512, '\n');
        } while (mapChoice < 1 || mapChoice > numberMaps || isnan(mapChoice));
        return std::string(mapList.at(mapChoice - 1));
    }
    return "";
}

/**
 * Prompts the user for the number of players
 * @return the number of players
 */
static int choosePlayerNumber() {
    unsigned int playerChoice;
    do {
        cout << "please choose a number of players between 2 and 6 :\n";
        cin >> playerChoice;
        cin.clear();
        cin.ignore(512, '\n');
    } while (playerChoice < 2 || playerChoice > 6 || isnan(playerChoice));
    return int(playerChoice);
}

/**
 * Creates the players and assigns countries to them
 * @param numPlayers the number of players
 * @param map the map object
 * @return the vector of players
 */
static vector<Player*>* initPlayers(int numPlayers, Map* map) {
    vector<vector<Map::Country*>> countriesPerPlayer;
    countriesPerPlayer.reserve(numPlayers);
    //split up the countries by the number of players
    for (int i = 0; i < numPlayers; i++) {
        countriesPerPlayer.emplace_back();
    }
    //randomize map countries
    vector<Map::Country*>* randomizedCountries = map->getMapCountries();
    std::shuffle(randomizedCountries->begin(), randomizedCountries->end(), std::mt19937(std::random_device()()));
    for (unsigned long i = 0; i < randomizedCountries->size(); i++) {
        Map::Country* currCountry = randomizedCountries->at(i);
        currCountry->setPlayerOwnerID(int(i) % numPlayers);
        countriesPerPlayer[i % numPlayers].push_back(currCountry);
    }

    auto* players = new vector<Player*>();
    players->reserve(numPlayers);
    //create the players with their respective list of countries created above
    for (int i = 0; i < numPlayers; i++) {
        auto* player = new Player(countriesPerPlayer[i], new Hand(), new DiceRoller(), i);
        new PhaseObserver(player);
        players->push_back(player);
    }
    std::shuffle(players->begin(), players->end(), std::mt19937(std::random_device()()));
    return players;
}

/**
 * Get the total number of armies for the game
 * @param numberOfPlayers the number of players
 * @return the number of armies per player in the game
 */
static int getNumberOfArmies(int numberOfPlayers) {
    switch (numberOfPlayers) {
        case 2:
            return 40;
        case 3:
            return 35;
        case 4:
            return 30;
        case 5:
            return 25;
        case 6:
            return 20;
        default:
            return 0;
    }
}

/**
 * Prompt each player for army distribution
 */
void GameLoop::distributeArmies() {
    int numberOfPlayers = allPlayers->size();
    int numberOfArmies = getNumberOfArmies(numberOfPlayers);
    int currentPlayerPosition = 0;
    Player* currentPlayer = allPlayers->at(currentPlayerPosition);
    int counter = 0;
    cout << "\nEach player has " << numberOfArmies << " armies to place on their countries. \n";
    while (counter < numberOfPlayers) {
        cout << "Placing 1 army per country." << std::endl;
        for (auto& country : *currentPlayer->getOwnedCountries()) {
            country->setNumberOfTroops(1);
        }
        cout << "Player " << currentPlayer->getPlayerId()
             << " , please place your armies. Here is your list of countries :\n";
        for (unsigned long i = 1; i <= currentPlayer->getOwnedCountries()->size(); i++) {
            cout << i << " - " << currentPlayer->getOwnedCountries()->at(i - 1)->getCountryName() << "\n";
        }
        for (int i = currentPlayer->getOwnedCountries()->size() + 1; i <= numberOfArmies; i++) {
            int countryToPlaceOn;
            do {
                cout << "\nWhere do you place army number " << i;
                cin >> countryToPlaceOn;
                cin.clear();
                cin.ignore(512, '\n');
            } while (countryToPlaceOn < 1 || countryToPlaceOn > int(currentPlayer->getOwnedCountries()->size()) ||
                     isnan(countryToPlaceOn));
            //increment the number of troops on the selected country
            Map::Country* currCountry = currentPlayer->getOwnedCountries()->at(countryToPlaceOn - 1);
            currCountry->setNumberOfTroops(currCountry->getNumberOfTroops() + 1);
        }
        currentPlayerPosition++;
        currentPlayerPosition = currentPlayerPosition % numberOfPlayers;
        currentPlayer = allPlayers->at(currentPlayerPosition);
        counter++;
    }
}

/**
 * Game start phase
 */
void GameLoop::start() {

    std::string mapToLoad;
    int numberOfPlayers;
    Map* gameMap;
    do {
        mapToLoad = chooseMap();
        numberOfPlayers = choosePlayerNumber();

        MapLoader myLoader = MapLoader(mapToLoad);
        gameMap = myLoader.readMapFile();
        if (gameMap == nullptr || !gameMap->testConnected()) {
            cout << "\nThere was an error loading the game board. Try another mapfile.\n";
            continue;
        } else if (gameMap->getMapCountries()->size() < numberOfPlayers) {
            std::cout << "The selected map with " << gameMap->getMapCountries()->size() << " cannot support "
                      << numberOfPlayers << " players. Please try again." << std::endl;
        } else {
            break;
        }
    } while (true);

    std::vector<Player*>* gamePlayers = initPlayers(numberOfPlayers, gameMap);
    Deck* gameDeck = new Deck(gameMap->getMapCountries()->size());
    gameDeck->createDeck();
    gameMap->printMap();

    GameLoop::initInstance(gameMap, gamePlayers, gameDeck);
}



