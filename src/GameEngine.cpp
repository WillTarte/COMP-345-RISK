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
int GameLoop::loop(int maxTurn) {
    bool gameNotDone = true;
    int currentPlayerPosition = 0;
    Player* currentPlayer = nullptr;

    //limit or not number of turns
    int turnsLeft = -1;
    bool limitTurns = false;
    if(maxTurn != -1){
        turnsLeft = maxTurn * int(allPlayers->size());
        limitTurns = true;
    }

    do {
        turnsLeft--;
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
        }else{
            continue;
        }
        //return -1 if draw
        if(limitTurns && turnsLeft <= 0){
            return -1;
        }
    } while (gameNotDone);

    //return winner ID
    int winnerPlayerID = 99;
    for (auto* p : *GameLoop::getInstance()->getAllPlayers()) {
        if (p->getOwnedCountries()->size() == GameLoop::getInstance()->getGameMap()->getMapCountries()->size()) {
            winnerPlayerID = p->getPlayerId();
        }
    }
    currentPlayer->notifyAll();
    cout.flush();
    return winnerPlayerID;
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
            //increment to match with switch case bellow
            strat = playerRoles.at(i);
            strat++;
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
 * Center the text in the column
 * @param str
 * @param width
 * @return
 */
std::string centerOut(const string& str, const int width) {
    stringstream ss, spaces;
    int padding = width - int(str.size());                 // count excess room to pad
    for (int i = 0; i < padding / 2; ++i)
        spaces << " ";
    ss << spaces.str() << str << spaces.str();    // format with padding
    if (padding > 0 && padding % 2 != 0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}


static void printGameReport(vector<std::string> maps, vector<char> players, int games, int draw, vector<int> winners){
    std::cout << "\n\n--------------------------------------------------------\n";
    std::cout << "--------------------Game Statistics---------------------\n";
    std::cout << "--------------------------------------------------------\n\n";

    //print maps
    std::cout << "M: ";
    for(int i = 0; i < int(maps.size()); i++){
        if(i == int(maps.size()-1)){
            std::cout << maps.at(i) << std::endl;
        }else{
            std::cout << maps.at(i) << ", ";
        }
    }

    //normalize and print players
    vector<std::string> playerStrats;
    std::cout << "P: ";
    for(int i = 0; i < int(players.size()); i++){
        std::string currPlayer = std::to_string(players.at(i));
        //normalize
        if(currPlayer == "a"){
            playerStrats.emplace_back("Aggresive");
        }else if(currPlayer == "b"){
            playerStrats.emplace_back("Passive");
        }else if(currPlayer == "c"){
            playerStrats.emplace_back("Random");
        }else if(currPlayer == "d"){
            playerStrats.emplace_back("Cheater");
        }else{
            playerStrats.emplace_back("Unknown");
        }
        //print
        if(i == int(players.size()-1)){
            std::cout << playerStrats.at(i) << "." << std::endl;
        }else{
            std::cout << playerStrats.at(i) << ", ";
        }
    }

    //print other info
    std::cout << "G: " << games;
    std::cout << "D: " << draw;

    //print table
    for(int i = 0; i < int(maps.size()); i++){
        for(int j = 0; j < games; j++){
            if(j == 0 &&  i == 0){
                std::cout << centerOut(" ", 10) << " | ";
            }else if(i == 0){
                std::string to_print = "Game " + std::to_string(i+1);
                std::cout << centerOut(to_print, 10) << " | ";
            }else if(j == 0){
                std::cout << centerOut(maps.at(i), 10) << " | ";
            }else{
                std::string thisWinner;
                int winnerIndex = winners.at(i + (j*3));
                if(winnerIndex == -1){
                    thisWinner = "Draw";
                }else{
                    thisWinner = playerStrats.at(winnerIndex);
                }
                std::cout << centerOut(thisWinner,10) << " | ";
            }
            for (int g = 0; g < 56; g++) {
                std::cout << "-";
            }
            std::cout << std::endl;
        }
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
        //place one troop per country
        for (auto& country : *currentPlayer->getOwnedCountries()) {
            country->setNumberOfTroops(1);
        }
        //manual place for humans, automatic for bots
        if(currentPlayer->getStrategy()->getStrategyName() == "HUMAN"){
            cout << "Placing 1 army per country." << std::endl;
            cout << "Player " << currentPlayer->getPlayerId()
                 << " , please place your armies. Here is your list of countries :\n";
            for (unsigned long i = 1; i <= currentPlayer->getOwnedCountries()->size(); i++) {
                cout << i << " - " << currentPlayer->getOwnedCountries()->at(i - 1)->getCountryName() << "\n";
            }
            for (int i = int(currentPlayer->getOwnedCountries()->size() + 1); i <= numberOfArmies; i++) {
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
        }else{
            //define random device and range
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<> distr(1, int(currentPlayer->getOwnedCountries()->size()));
            //place troops randomly
            for (int i = int(currentPlayer->getOwnedCountries()->size() + 1); i <= numberOfArmies; i++) {
                int countryToPlaceOn = distr(eng);
                //increment the number of troops on the selected country
                Map::Country* currCountry = currentPlayer->getOwnedCountries()->at(countryToPlaceOn - 1);
                currCountry->setNumberOfTroops(currCountry->getNumberOfTroops() + 1);
            }
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
         }else if (int(gameMap->getMapCountries()->size()) < numberOfPlayers) {
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
     vector<int> winners;
     for(auto currentMap : mapList){
         for(int i = 0; i < numGamesToPlay; i++){
             //init
             std::vector<Player*>* gamePlayers = initPlayers(numberOfPlayers, currentMap, playerStrats);
             Deck* gameDeck = new Deck(currentMap->getMapCountries()->size());
             gameDeck->createDeck();
             GameLoop::initInstance(currentMap, gamePlayers, gameDeck);

             //run game
             GameLoop::getInstance()->distributeArmies();
             std::cout << "here";
             winners.push_back(GameLoop::getInstance()->loop(maxTurns));
             std::cout << "not here";
             GameLoop::resetInstance();

             //cleanup
             for(auto & gamePlayer : *gamePlayers){
                 delete(gamePlayer);
             }
             delete(gamePlayers);
             delete(gameDeck);
         }
     }
     //print report
     printGameReport(mapNames,playerStrats,numGamesToPlay,maxTurns,winners);
}

/**
 * Single game mode start phase
 */
void GameLoop::startSingle(bool demoMode) {
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
        } else if (int(gameMap->getMapCountries()->size()) < numberOfPlayers) {
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

    if(!demoMode){
        //run game
        GameLoop::getInstance()->distributeArmies();
        GameLoop::getInstance()->loop();
        GameLoop::resetInstance();
    }
}




