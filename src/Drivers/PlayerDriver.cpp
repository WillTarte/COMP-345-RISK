//
// Created by William Tarte on 10/8/2019.
//
#include "../include/GameEngine.h"
#include "../include/Map.h"
#include "../include/Cards.h"
#include "../include/Dice.h"
#include "../include/Player.h"
#include <iostream>
#include <vector>
#include <iterator>

//TODO - fix the driver using the new attack, fortify, and reinforce methods

bool test_Player_Constructor() {

    // Arrange
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    Map::Country* pCountry3 = &country3;
    ownedCountries1.push_back(pCountry1);
    ownedCountries1.push_back(pCountry2);
    ownedCountries1.push_back(pCountry3);
    Hand cards = Hand();
    Deck deck = Deck(3);
    deck.createDeck();
    deck.draw(cards);
    DiceRoller diceRoller = DiceRoller();
    const int playerId = 1;
    bool success = true;

    // Act
    Player player1 = Player(ownedCountries1, cards, diceRoller, playerId);

    // Assert
    if (player1.getOwnedCountries()->empty() || player1.getCards().getHand()->empty() ||
        playerId != player1.getPlayerId()) {
        success = false;
    }
    return success;
}

bool test_Player_getOwnedCountries(bool verbose = false) {

    // Arrange
    bool success = true;
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    Map::Country* pCountry3 = &country3;
    ownedCountries1.push_back(pCountry1);
    ownedCountries1.push_back(pCountry2);
    ownedCountries1.push_back(pCountry3);

    // Act
    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 0);

    // Assert
    if (player1.getOwnedCountries()->empty()) {
        success = false;
    }
    if (verbose) {
        std::cout << "\033[35m";
        std::cout << "\nThe player owns countries: ";
        for (auto& i : *player1.getOwnedCountries()) {
            std::cout << i->getCountryName() << " ";
        }
        std::cout << "\033[31m" << std::endl;
    }
    return success;
}

bool test_Player_getHand(bool verbose = false) {

    // Arrange
    bool success = true;
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();
    Hand cards = Hand();
    Deck deck = Deck(3);
    deck.createDeck();
    deck.draw(cards);
    deck.draw(cards);
    deck.draw(cards);
    deck.draw(cards);

    // Act
    Player player1 = Player(ownedCountries1, cards, DiceRoller(), 1);

    // Assert
    if (player1.getCards().getHand()->empty()) {
        success = false;
    }

    if (verbose) {
        std::cout << "\n" << "\033[35m";
        for (auto& i : *player1.getCards().getHand()) {
            switch ((int) i) {
                case 0:
                    std::cout << "player1 has a card " << "INFANTRY" << std::endl;
                    break;
                case 1:
                    std::cout << "player1 has a card " << "ARTILLERY" << std::endl;
                    break;
                case 2:
                    std::cout << "player1 has a card " << "CAVALRY" << std::endl;
                    break;
            }
        }
        std::cout << "\033[31m";
    }
    return success;
}

bool test_Player_getDiceRoller(bool verbose = false) {

    // Arrange
    bool success = true;
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();

    // Act
    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);

    // Assert
    if (player1.getDiceRoller().roll(1).empty()
        || player1.getDiceRoller().roll(2).size() != 2
        || player1.getDiceRoller().getHistory().empty()
        || player1.getDiceRoller().getPercentages().empty()) {

        success = false;
    }

    if (verbose) {
        std::cout << "\n" << "\033[35m";
        std::cout << "player1 rolled ";
        std::vector<int> rolls = player1.getDiceRoller().roll(1);
        std::copy(rolls.begin(), rolls.end(), std::ostream_iterator<int>(std::cout));
        std::cout << std::endl;
        std::cout << "player1 rolled ";
        rolls = player1.getDiceRoller().roll(2);
        std::copy(rolls.begin(), rolls.end(), std::ostream_iterator<int>(std::cout));
        std::cout << std::endl;
        std::cout << "player1 rolled ";
        rolls = player1.getDiceRoller().roll(3);
        std::copy(rolls.begin(), rolls.end(), std::ostream_iterator<int>(std::cout));
        std::cout << "\033[31m\n";
    }
    return success;
}
/*
bool test_Player_fortify(bool verbose = false) {

    // Arrange
    const int numArmies = 4;
    bool success = true;
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);
    country1.setPlayerOwnerID(1);
    country2.setPlayerOwnerID(1);
    country3.setPlayerOwnerID(1);
    country1.setNumberOfTroops(numArmies);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    Map::Country* pCountry3 = &country3;
    ownedCountries1.push_back(pCountry1);
    ownedCountries1.push_back(pCountry2);
    ownedCountries1.push_back(pCountry3);
    country1.pAdjCountries->push_back(pCountry2);
    country2.pAdjCountries->push_back(pCountry1);
    country2.pAdjCountries->push_back(pCountry3);
    country3.pAdjCountries->push_back(pCountry2);

    // Act & Assert
    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);

    if (player1.fortify(country1, country2, numArmies - 1) == PlayerAction::FAILED) {
        success = false;
    }
    if (country1.getNumberOfTroops() >= numArmies) {
        success = false;
    }

    if (verbose) {
        std::cout << "\033[35m";
        std::cout << "Country " << country1.getCountryName() << " had " << numArmies << " armies" << std::endl;
        std::cout << "Country " << country1.getCountryName() << " now has " << country1.getNumberOfTroops() << " armies"
                  << std::endl;
        std::cout << "Country " << country2.getCountryName() << " had " << 0 << " armies" << std::endl;
        std::cout << "Country " << country2.getCountryName() << " now has " << country2.getNumberOfTroops() << " armies"
                  << std::endl;
        std::cout << "\033[31m";
    }

    return success;
}
*/

bool test_Player_attack() {

    // Arrange
    const int numArmies = 4;
    bool success = true;
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();
    std::vector<Map::Country*> ownedCountries2 = std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 1);
    Map::Country country2 = Map::Country(1, "country2", 1);
    country1.setPlayerOwnerID(1);
    country2.setPlayerOwnerID(2);
    country1.setNumberOfTroops(numArmies);
    country2.setNumberOfTroops(numArmies - 2);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    ownedCountries1.push_back(pCountry1);
    ownedCountries2.push_back(pCountry2);
    country1.getAdjCountries()->push_back(pCountry2);
    country2.getAdjCountries()->push_back(pCountry1);

    vector<Map::Country*> countries = {pCountry1, pCountry2};
    vector<Map::Country*>* cRef = &countries;

    Map m = Map("", vector<vector<string>>());
    Map* mapRef = &m;
    m.setMapCountries(cRef);

    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);
    Player player2 = Player(ownedCountries2, Hand(), DiceRoller(), 2);

    std::vector<Player*> players = {&player1, &player2};
    GameLoop::initInstance(mapRef, &players);

    // Act & Assert
    if (player1.attack() == PlayerAction::FAILED) {
        success = false;
    }

    return success;
}

bool test_Player_reinforce(bool verbose = false) {
    bool success = true;
    const int numArmies = 4;

    auto continents = std::vector<Map::Continent*>();
    auto countries = std::vector<Map::Country*>();

    auto continent = new Map::Continent("North America", numArmies);
    auto country = new Map::Country(0, "Canada", 0);

    continents.push_back(continent);
    countries.push_back(country);
    continent->addCountry(country);

    Map map = Map("test", continents, countries);
    Map::initInstance(&map);

    // Act & Assert
    Player player1 = Player(*map.getMapCountries(), Hand(), DiceRoller(), 1);

    if (player1.reinforce() == PlayerAction::FAILED) {
        success = false;
    }

    auto country1 = *map.getMapCountries()[0][0];
    if (verbose) {
        std::cout << "\033[35m";
        std::cout << "player1's " << country1.getCountryName() << " had " << numArmies << " armies" << std::endl;
        std::cout << "player1's " << country1.getCountryName() << " now has " << country1.getNumberOfTroops()
                  << " armies" << std::endl;
        std::cout << "\033[31m";
    }

    return success;
}

bool test_fortify() {

    bool success;

    // Create elements for player1
    std::vector<Map::Country*> ownedCountries1 = std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(2, "country3", 1);
    Map::Country country4 = Map::Country(3, "country4", 1);
    Map::Country country5 = Map::Country(4, "country5", 1);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    Map::Country* pCountry3 = &country3;
    Map::Country* pCountry4 = &country4;
    Map::Country* pCountry5 = &country5;
    ownedCountries1.push_back(pCountry1);
    ownedCountries1.push_back(pCountry2);
    ownedCountries1.push_back(pCountry3);
    ownedCountries1.push_back(pCountry4);
    ownedCountries1.push_back(pCountry5);
    country1.setPlayerOwnerID(1);
    country2.setPlayerOwnerID(1);
    country3.setPlayerOwnerID(1);
    country4.setPlayerOwnerID(1);
    country5.setPlayerOwnerID(1);
    country1.getAdjCountries()->push_back(pCountry2);
    country2.getAdjCountries()->push_back(pCountry1);
    country2.getAdjCountries()->push_back(pCountry3);
    country5.getAdjCountries()->push_back(pCountry1);
    Hand cards = Hand();
    Deck deck = Deck(4);
    deck.createDeck();
    deck.draw(cards);
    DiceRoller diceRoller = DiceRoller();
    const int playerId = 1;

    // Create elements for player2
    std::vector<Map::Country*> ownedCountries2 = std::vector<Map::Country*>();
    Map::Country country6 = Map::Country(5, "country6", 0);
    Map::Country country7 = Map::Country(6, "country7", 0);
    Map::Country country8 = Map::Country(7, "country8", 1);
    Map::Country country9 = Map::Country(8, "country9", 1);
    Map::Country country10 = Map::Country(9, "country10", 1);
    Map::Country* pCountry6 = &country6;
    Map::Country* pCountry7 = &country7;
    Map::Country* pCountry8 = &country8;
    Map::Country* pCountry9 = &country9;
    Map::Country* pCountry10 = &country10;
    ownedCountries2.push_back(pCountry6);
    ownedCountries2.push_back(pCountry7);
    ownedCountries2.push_back(pCountry8);
    ownedCountries2.push_back(pCountry9);
    ownedCountries2.push_back(pCountry10);
    country6.setPlayerOwnerID(2);
    country7.setPlayerOwnerID(2);
    country8.setPlayerOwnerID(2);
    country9.setPlayerOwnerID(2);
    country10.setPlayerOwnerID(2);
    country6.getAdjCountries()->push_back(pCountry7);
    country7.getAdjCountries()->push_back(pCountry6);
    country7.getAdjCountries()->push_back(pCountry1);
    country8.getAdjCountries()->push_back(pCountry9);
    country9.getAdjCountries()->push_back(pCountry10);
    country10.getAdjCountries()->push_back(pCountry2);
    country1.getAdjCountries()->push_back(pCountry7);
    country2.getAdjCountries()->push_back(pCountry6);
    country5.getAdjCountries()->push_back(pCountry9);
    Hand cards2 = Hand();
    Deck deck2 = Deck(4);
    deck2.createDeck();
    deck2.draw(cards);
    DiceRoller diceRoller2 = DiceRoller();
    const int playerId2 = 2;

    country1.setNumberOfTroops(10);
    country2.setNumberOfTroops(4);
    country3.setNumberOfTroops(1);
    country4.setNumberOfTroops(5);
    country5.setNumberOfTroops(15);
    country6.setNumberOfTroops(3);
    country7.setNumberOfTroops(21);
    country8.setNumberOfTroops(11);
    country9.setNumberOfTroops(2);
    country10.setNumberOfTroops(1);

    // Create players
    Player player1 = Player(ownedCountries1, cards, diceRoller, playerId);
    Player player2 = Player(ownedCountries2, cards2, diceRoller2, playerId2);

    // Assert
    success = !(player1.fortify() == PlayerAction::FAILED);

    //Print each player's countries and armies to verify that the method worked
    std::cout << "\nPlayer1's countries and armies\n" << std::endl;
    for (const auto i : *player1.getOwnedCountries()){
        std::cout << i->getCountryName() << " with ";
        std::cout << i->getNumberOfTroops() << " armies and ";
        std::cout << i->getAdjCountries()->size() << " adjacent country/ies ; \n";
        for (auto& pAdjCountry : *i->getAdjCountries()) {
            std::cout << "    - " << pAdjCountry->getCountryName() << " (owned by Player " << pAdjCountry->getPlayerOwnerID() << " )\n";
        }
    }

    std::cout << "\nPlayer2's countries and armies\n" << std::endl;
    for (const auto i : *player2.getOwnedCountries()){
        std::cout << i->getCountryName() << " with ";
        std::cout << i->getNumberOfTroops() << " armies and ";
        std::cout << i->getAdjCountries()->size() << " adjacent country/ies ; \n";
        for (auto& pAdjCountry : *i->getAdjCountries()) {
            std::cout << "    - " << pAdjCountry->getCountryName() << " (owned by Player " << pAdjCountry->getPlayerOwnerID() << " )\n";
        }
    }

    return success;
}

////////////////////////////////////////////////////////////
////////////////////// Run Tests ///////////////////////////
////////////////////////////////////////////////////////////

#define assert(c, m, s) "\nTesting " << c << " " << m << "() method: " \
    << (s ? "\033[32mPass" : "\033[31mFail") << "\033[30m" << std::endl

int main() {

    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------ Running tests for Player class ------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m" << std::endl;

    std::cout << assert("Player", "constructor", test_Player_Constructor()) << std::endl;

    std::cout << assert("Player", "getOwnedCountries", test_Player_getOwnedCountries(true)) << std::endl;

    std::cout << assert("Player", "fortify",test_fortify()) << std::endl;

    std::cout << assert("Player", "getCards", test_Player_getHand(true)) << std::endl;

    std::cout << assert("Player", "getDiceRoller", test_Player_getDiceRoller(true)) << std::endl;

    std::cout << assert("Player", "attack", test_Player_attack()) << std::endl;

    std::cout << assert("Player", "reinforce", test_Player_reinforce(true)) << std::endl;

}
