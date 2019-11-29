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

    do {

        currentPlayer = allPlayers->at(currentPlayerPosition);
        if(currentPlayer->getOwnedCountries()->empty()) {
            continue;
        }

        cout << "\u001b[35m";
        currentPlayer->reinforce();

        cout << "\u001b[33m";
        currentPlayer->attack();

        cout << "\u001b[34m";
        currentPlayer->fortify();

        gameNotDone = !isGameDone(currentPlayer,gameMap);

        if (gameNotDone) {
            currentPlayerPosition++;
            if (isRoundFinished(currentPlayerPosition)) {
                currentPlayerPosition = 0;
                // for demo - give all countries to first player at the end of the round
                currentPlayer->setOwnedCountries(gameMap->getMapCountries());
                gameNotDone = false;
            }
        }
    } while (gameNotDone);

    currentPlayer->notifyAll();
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
bool GameLoop::isGameDone(Player* currentPlayer,Map* currMap) {
    return currentPlayer->getOwnedCountries()->size() == currMap->getMapCountries()->size();
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
static int choosePlayerNumber(int minPlayers, int maxPlayers) {
    int playerChoice;
    do {
        cout << "please choose a number of players between " << minPlayers << " and " << maxPlayers << " :\n";
        cin >> playerChoice;
        cin.clear();
        cin.ignore(512, '\n');
    } while (playerChoice < minPlayers || playerChoice > maxPlayers || isnan(playerChoice));
    return int(playerChoice);
}

static bool loadOtherMap() {
    char playerChoice = 'z';
    do {
        cout << "\nWould you like to add a map to the tournament (y / n) ?";
        cin >> playerChoice;
        cin.clear();
        cin.ignore(512, '\n');
    } while (playerChoice != 'y' && playerChoice != 'n');
    return playerChoice == 'y';
}

static vector<char> choosePlayerStrats(int numPlayers) {
    vector<char> strats;
    int counter = 1;
    char playerChoice;
    do {
        cout << "What strategy should player " << counter << " use? a) aggresive b) passive c) random d) cheater"  << std::endl;
        cin >> playerChoice;
        cin.clear();
        cin.ignore(512, '\n');
        if(playerChoice != 'a' && playerChoice != 'b' && playerChoice != 'c' && playerChoice != 'd'){
            continue;
        }else if(std::find(strats.begin(), strats.end(), playerChoice) != strats.end()){
            cout << "Please choose a different strategy, this one is already taken by another player"  << std::endl;
            continue;
        }else{
            counter++;
            strats.push_back(playerChoice);
        }
        if(counter > numPlayers){
            break;
        }
    } while (true);
    return strats;
}

static int chooseNumGamesToPlay() {
    int numGames;
    do {
        cout << "please choose the number of games to be played on each map (between 1 and 5) : \n";
        cin >> numGames;
        cin.clear();
        cin.ignore(512, '\n');
    } while (numGames < 1 || numGames > 5 || isnan(numGames));
    return int(numGames);
}

static int chooseMaxTurns() {
    int numTurns;
    do {
        cout << "please choose the maximum number of turns to be played on each map (between 3 and 50) : \n";
        cin >> numTurns;
        cin.clear();
        cin.ignore(512, '\n');
    } while (numTurns < 3 || numTurns > 50 || isnan(numTurns));
    return int(numTurns);
}


/**
 * Creates the players and assigns countries to them
 * @param numPlayers the number of players
 * @param map the map object
 * @return the vector of players
 */
static vector<Player*>* initPlayers(int numPlayers, Map* map, vector<char> playerRoles = {}) {
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

        char strat = 0;

        //support tournament role
        if(playerRoles.empty()){
            std::cout << "What strategy should player " << i << " use? a) Human b) aggresive c) passive d) random e) cheater"  << std::endl;
            std::cin >> strat;
        }else{
            strat = playerRoles.at(i)++;
        }

        switch(strat) {
            case 'a': player->setPlayerStrategy(Strategies::HUMAN_PLAYER); break;
            case 'b': player->setPlayerStrategy(Strategies::AGGRESSIVE_BOT); break;
            case 'c': player->setPlayerStrategy(Strategies::BENEVOLENT_BOT); break;
            case 'd': player->setPlayerStrategy(Strategies::RANDOM_BOT); break;
            case 'e': player->setPlayerStrategy(Strategies::CHEATER_BOT); break;
            default: {
                player->setPlayerStrategy(Strategies::HUMAN_PLAYER); break;
            }
        }
        new StatsObserver(player);
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
// TODO: based on player strategy, distribute armies (i.e. automaticaly distributed or human player decided)
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
                cout << "\nWhere do you place army number " << i << ": ";
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
 * Choose Game start phase
 */
void GameLoop::start() {
    int gameModeChoice;
    do {
        cout << "\nWhich game mode do you want to play? \n 1 - Single game \n 2 - Tournament \n";
        cin >> gameModeChoice;
        cin.clear();
        cin.ignore(512, '\n');
    } while (gameModeChoice < 1 || gameModeChoice > 2 || isnan(gameModeChoice));

    // init gamemode
    switch(gameModeChoice){
        case 1:
            startSingle();
            break;
        case 2:
            startTournament();
            break;
        default:
            //should never get here
            startSingle();
            break;
    }
}

/**
 * Tournament mode start phase
 */
 void GameLoop::startTournament() {
     int numberOfPlayers = choosePlayerNumber(2,4);
     vector<char> playerStrats = choosePlayerStrats(numberOfPlayers);
     int numMaps = 0;
     vector<Map*> mapList;
     vector<string> mapNames;
     do{
         //which map to load
         std::string mapToLoad;
         Map* gameMap;
         if(numMaps == 0 || loadOtherMap()){
             mapToLoad = chooseMap();
         }else{
             break;
         }
         //load the map (try to use both loaders, to accept both map types)
         MapLoader myLoader = MapLoader(mapToLoad);
         gameMap = myLoader.readMapFile();
         if(gameMap == nullptr){
             AlternativeLoader altLoader = AlternativeLoader(mapToLoad);
             gameMap = altLoader.altReadMapFile();
         }
         //test map
         if(gameMap == nullptr || !gameMap->testConnected()){
             cout << "\nThere was an error loading the game board. Try another mapfile.\n";
             continue;
         }else if (gameMap->getMapCountries()->size() < numberOfPlayers) {
             std::cout << "The selected map with " << gameMap->getMapCountries()->size() << " cannot support "
                       << numberOfPlayers << " players. Please try again." << std::endl;
             continue;
         } else if(std::find(mapNames.begin(), mapNames.end(), mapToLoad) != mapNames.end()){
             cout << "\nThis map was already chosen, choose another map file.\n";
             continue;
         }else {
             numMaps++;
             mapList.push_back(gameMap);
             mapNames.push_back(mapToLoad);
         }
         //bail
         if(numMaps >= 5){
             cout << "\nThe maximum number of maps was created.\n";
            break;
         }
     }while(true);

     //fetch game settings
     int numGamesToPlay = chooseNumGamesToPlay();
     int maxTurns = chooseMaxTurns();

     //run the games
     for(int i = 1; i <= numGamesToPlay; i++){
         //init
         Map* currentMap = mapList.at(i-1);
         std::vector<Player*>* gamePlayers = initPlayers(numberOfPlayers, currentMap, playerStrats);
         Deck* gameDeck = new Deck(currentMap->getMapCountries()->size());
         gameDeck->createDeck();
         GameLoop::initInstance(currentMap, gamePlayers, gameDeck);

         //run game
         GameLoop::getInstance()->distributeArmies();
         GameLoop::getInstance()->loop();
         GameLoop::resetInstance();

         //cleanup
         for(auto & gamePlayer : *gamePlayers){
             delete(gamePlayer);
         }
         delete(gamePlayers);
         delete(gameDeck);
     }
}

/**
 * Single game mode start phase
 */
void GameLoop::startSingle() {
    std::string mapToLoad;
    int numberOfPlayers;
    Map* gameMap;
    do {
        mapToLoad = chooseMap();
        numberOfPlayers = choosePlayerNumber(2,6);
        //load the map (try to use both loaders, to accept both map types)
        MapLoader myLoader = MapLoader(mapToLoad);
        gameMap = myLoader.readMapFile();
        if(gameMap == nullptr){
            AlternativeLoader altLoader = AlternativeLoader(mapToLoad);
            gameMap = altLoader.altReadMapFile();
        }
        if (gameMap == nullptr || !gameMap->testConnected()) {
            cout << "There was an error loading the game board. Try another mapfile.\n";
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

    cout << "Number of cards in the deck: " << *GameLoop::getInstance()->getGameDeck()->getNumberOfCards() << endl;
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
}



