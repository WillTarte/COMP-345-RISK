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

//NOTE: because of requirements for the GameEngine, the test methods for the player behaviors do not work anymore

bool test_Player_Constructor() {

    // Arrange
    auto* ownedCountries1 = new std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    Map::Country* pCountry3 = &country3;
    ownedCountries1->push_back(pCountry1);
    ownedCountries1->push_back(pCountry2);
    ownedCountries1->push_back(pCountry3);
    auto* cards = new Hand();
    Deck deck = Deck(3);
    deck.createDeck();
    deck.draw(*cards);
    auto* diceRoller = new DiceRoller();
    const int playerId = 1;
    bool success = true;

    // Act
    auto* player1 = new Player(ownedCountries1, cards, diceRoller, playerId);

    // Assert
    if (player1->getOwnedCountries()->empty() || player1->getCards()->getHand()->empty() ||
        playerId != player1->getPlayerId()) {
        success = false;
    }

    delete player1;
    return success;
}

bool test_Player_getOwnedCountries(bool verbose = false) {

    // Arrange
    bool success = true;
    auto* ownedCountries1 = new std::vector<Map::Country*>();
    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);
    Map::Country* pCountry1 = &country1;
    Map::Country* pCountry2 = &country2;
    Map::Country* pCountry3 = &country3;
    ownedCountries1->push_back(pCountry1);
    ownedCountries1->push_back(pCountry2);
    ownedCountries1->push_back(pCountry3);
    auto* cards = new Hand();
    auto* diceRoller = new DiceRoller();

    // Act
    auto* player1 = new Player(ownedCountries1, cards, diceRoller, 0);

    // Assert
    if (player1->getOwnedCountries()->empty()) {
        success = false;
    }
    if (verbose) {
        std::cout << "\033[35m";
        std::cout << "\nThe player owns countries: ";
        for (auto& i : *player1->getOwnedCountries()) {
            std::cout << i->getCountryName() << " ";
        }
        std::cout << "\033[31m" << std::endl;
    }
    delete player1;
    return success;
}

bool test_Player_getHand(bool verbose = false) {

    // Arrange
    bool success = true;
    auto* ownedCountries1 = new std::vector<Map::Country*>();
    auto* cards = new Hand();
    Deck deck = Deck(3);
    deck.createDeck();
    deck.draw(*cards);
    deck.draw(*cards);
    deck.draw(*cards);
    deck.draw(*cards);
    auto* diceRoller = new DiceRoller();

    // Act
    auto* player1 = new Player(ownedCountries1, cards, diceRoller, 1);

    // Assert
    if (player1->getCards()->getHand()->empty()) {
        success = false;
    }

    if (verbose) {
        std::cout << "\n" << "\033[35m";
        for (auto& i : *player1->getCards()->getHand()) {
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
    delete player1;
    return success;
}

bool test_Player_getDiceRoller(bool verbose = false) {

    // Arrange
    bool success = true;
    auto* ownedCountries1 = new std::vector<Map::Country*>();
    auto* cards = new Hand();
    auto* diceRoller = new DiceRoller();

    // Act
    auto* player1 = new Player(ownedCountries1, cards, diceRoller, 1);

    // Assert
    if (player1->getDiceRoller()->roll(1).empty()
        || player1->getDiceRoller()->roll(2).size() != 2
        || player1->getDiceRoller()->getHistory().empty()
        || player1->getDiceRoller()->getPercentages().empty()) {

        success = false;
    }

    if (verbose) {
        std::cout << "\n" << "\033[35m";
        std::cout << "player1 rolled ";
        std::vector<int> rolls = player1->getDiceRoller()->roll(1);
        std::copy(rolls.begin(), rolls.end(), std::ostream_iterator<int>(std::cout));
        std::cout << std::endl;
        std::cout << "player1 rolled ";
        rolls = player1->getDiceRoller()->roll(2);
        std::copy(rolls.begin(), rolls.end(), std::ostream_iterator<int>(std::cout));
        std::cout << std::endl;
        std::cout << "player1 rolled ";
        rolls = player1->getDiceRoller()->roll(3);
        std::copy(rolls.begin(), rolls.end(), std::ostream_iterator<int>(std::cout));
        std::cout << "\033[31m\n";
    }
    delete player1;
    return success;
}

bool test_Player_fortify(bool verbose = false) {

    bool success = true;
    GameLoop::start();
    GameLoop::getInstance()->distributeArmies(); // because you need armies to fortify

    int numArmiesC1 = GameLoop::getInstance()->getAllPlayers()->at(0)->getOwnedCountries()->at(0)->getNumberOfTroops();
    auto* neighbour = new Map::Country(99, "neighbour", 1);
    neighbour->setPlayerOwnerID(GameLoop::getInstance()->getAllPlayers()->at(0)->getPlayerId());
    int numArmiesC2 = neighbour->getNumberOfTroops();

    GameLoop::getInstance()->getAllPlayers()->at(0)->getOwnedCountries()->at(0)->getAdjCountries()->push_back(neighbour);

    if (GameLoop::getInstance()->getAllPlayers()->at(0)->fortify() == PlayerAction::FAILED) {
        success = false;
    }

    if (verbose) {
        Map::Country* c1 = GameLoop::getInstance()->getAllPlayers()->at(0)->getOwnedCountries()->at(0);
        std::cout << "\033[35m";
        std::cout << "Country " << c1->getCountryName() << " had " << numArmiesC1 << " armies" << std::endl;
        std::cout << "Country " << c1->getCountryName() << " now has " << c1->getNumberOfTroops() << " armies"
                  << std::endl;
        std::cout << "Country " << neighbour->getCountryName() << " had " << numArmiesC2 <<" armies" << std::endl;
        std::cout << "Country " << neighbour->getCountryName() << " now has " << neighbour->getNumberOfTroops() << " armies"
                  << std::endl;
        std::cout << "\033[31m";
    }
    GameLoop::resetInstance();
    return success;
}

bool test_Player_attack() {

    // Arrange
    bool success = true;
    GameLoop::start();
    GameLoop::getInstance()->distributeArmies(); // because you need armies to attack
    if (GameLoop::getInstance()->getAllPlayers()->at(0)->attack() == PlayerAction::FAILED) {
        success = false;
    }
    GameLoop::resetInstance();
    return success;
}


bool test_Player_reinforce(bool verbose = false) {

    // Arrange
    bool success = true;
    GameLoop::startSingle(true);
    Hand* pHand = GameLoop::getInstance()->getAllPlayers()->at(0)->getCards();
    pHand->getHand()->push_back(CardType::CAVALRY);
    pHand->getHand()->push_back(CardType::CAVALRY);
    pHand->getHand()->push_back(CardType::CAVALRY);
    pHand->getHand()->push_back(CardType::CAVALRY);
    pHand->getHand()->push_back(CardType::CAVALRY);

    int numArmiesC1 = GameLoop::getInstance()->getAllPlayers()->at(0)->getOwnedCountries()->at(0)->getNumberOfTroops();

    if (GameLoop::getInstance()->getAllPlayers()->at(0)->reinforce() == PlayerAction::FAILED) {
        success = false;
    }

    if (verbose) {
        for (auto* c: *GameLoop::getInstance()->getAllPlayers()->at(0)->getOwnedCountries()) {
            std::cout << "\033[35m";
            std::cout << "player1's " << c->getCountryName() << " had " << numArmiesC1 << " armies" << std::endl;
            std::cout << "player1's " << c->getCountryName() << " now has " << c->getNumberOfTroops()
                      << " armies" << std::endl;
            std::cout << "\033[31m";
        }
    }
    GameLoop::resetInstance();
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

    std::cout << assert("Player", "getCards", test_Player_getHand(true)) << std::endl;

    std::cout << assert("Player", "getDiceRoller", test_Player_getDiceRoller(true)) << std::endl;

    std::cout << assert("Player", "attack", test_Player_attack()) << std::endl;

    std::cout << assert("Player", "reinforce", test_Player_reinforce(true)) << std::endl;

    std::cout << assert("Player", "fortify", test_Player_fortify(true)) << std::endl;

}
