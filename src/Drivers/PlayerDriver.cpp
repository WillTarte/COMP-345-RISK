//
// Created by William Tarte on 10/8/2019.
//
#include <iostream>
#include <vector>
#include <Map.h>
#include <Cards.h>
#include <Dice.h>
#include <Player.h>
#include <iterator>

bool test_Player_Constructor() {

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();

    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);

    Map::Country *pCountry1 = &country1;
    Map::Country *pCountry2 = &country2;
    Map::Country *pCountry3 = &country3;

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

    Player player1 = Player(ownedCountries1, cards, diceRoller, playerId);

    if (player1.getOwnedCountries().empty() || player1.getHand().getHand()->empty() ||
        playerId != player1.getPlayerId()) {
        success = false;
    }

    return success;
}

bool test_Player_getOwnedCountries(bool verbose = false) {

    bool success = true;

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();

    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);

    Map::Country *pCountry1 = &country1;
    Map::Country *pCountry2 = &country2;
    Map::Country *pCountry3 = &country3;

    ownedCountries1.push_back(pCountry1);
    ownedCountries1.push_back(pCountry2);
    ownedCountries1.push_back(pCountry3);

    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 0);

    if (player1.getOwnedCountries().empty()) {
        success = false;
    }

    if (verbose) {
        std::cout << "\033[35m";
        std::cout << "\nThe player owns countries: ";
        for (unsigned int i = 0; i < player1.getOwnedCountries().size(); i++) {
            std::cout << player1.getOwnedCountries().at(i)->getCountryName() << " ";
        }
        std::cout << "\033[31m" << std::endl;
    }
    return success;
}

bool test_Player_getHand(bool verbose = false) {

    bool success = true;

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();

    Hand cards = Hand();
    Deck deck = Deck(3);
    deck.createDeck();
    deck.draw(cards);
    deck.draw(cards);
    deck.draw(cards);
    deck.draw(cards);

    Player player1 = Player(ownedCountries1, cards, DiceRoller(), 1);

    if (player1.getHand().getHand()->empty()) {
        success = false;
    }

    if (verbose) {
        std::cout << "\n" << "\033[35m";
        for (unsigned int i = 0; i < player1.getHand().getHand()->size(); i++) {

            switch ((int) player1.getHand().getHand()->at(i)) {

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

    bool success = true;

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();

    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);

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
        //std::cout << std::endl;

        std::cout << "\033[31m\n";
    }

    return success;
}

bool test_Player_reinforce(bool verbose = false) {

    const int numArmies = 4;
    bool success = true;

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();

    Map::Country country1 = Map::Country(0, "country1", 0);
    Map::Country country2 = Map::Country(1, "country2", 0);
    Map::Country country3 = Map::Country(3, "country3", 1);

    country1.setPlayerOwnerID(1);
    country2.setPlayerOwnerID(1);
    country3.setPlayerOwnerID(1);

    country1.setNumberOfTroops(numArmies);

    Map::Country *pCountry1 = &country1;
    Map::Country *pCountry2 = &country2;
    Map::Country *pCountry3 = &country3;

    ownedCountries1.push_back(pCountry1);
    ownedCountries1.push_back(pCountry2);
    ownedCountries1.push_back(pCountry3);

    country1.pAdjCountries->push_back(pCountry2);
    country2.pAdjCountries->push_back(pCountry1);
    country2.pAdjCountries->push_back(pCountry3);
    country3.pAdjCountries->push_back(pCountry2);

    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);

    if (player1.reinforce(pCountry1, pCountry2, numArmies - 1) == PlayerAction::FAILED) {
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

bool test_Player_attack(bool verbose = false) {

    const int numArmies = 4;
    const int numDefendingDice = 1;
    const int numAttackingDice = 3;
    bool success = true;

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();
    std::vector<Map::Country *> ownedCountries2 = std::vector<Map::Country *>();

    Map::Country country1 = Map::Country(0, "country1", 1);
    Map::Country country2 = Map::Country(1, "country2", 1);

    country1.setPlayerOwnerID(1);
    country2.setPlayerOwnerID(2);

    country1.setNumberOfTroops(numArmies);
    country2.setNumberOfTroops(numArmies - 3);

    Map::Country *pCountry1 = &country1;
    Map::Country *pCountry2 = &country2;

    ownedCountries1.push_back(pCountry1);
    ownedCountries2.push_back(pCountry2);

    country1.pAdjCountries->push_back(pCountry2);
    country2.pAdjCountries->push_back(pCountry1);

    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);
    Player player2 = Player(ownedCountries2, Hand(), DiceRoller(), 2);

    if (player1.attack(pCountry1, pCountry2, &player2, numAttackingDice, numDefendingDice) == PlayerAction::FAILED) {
        success = false;
    }

    if (verbose) {
        std::cout << "\n" << "\033[35m";
        std::cout << "player1 attacked player2's " << country2.getCountryName() << std::endl;
        std::cout << "player1 is using " << numAttackingDice << " dice " << " and player2 is using " << numDefendingDice
                  << std::endl;
        std::cout << "player1's " << pCountry1->getCountryName() << " had " << numArmies << std::endl;
        std::cout << "player2's " << pCountry2->getCountryName() << " had " << numArmies - 3 << std::endl;
        std::cout << "player1's " << pCountry1->getCountryName() << " now has " << pCountry1->getNumberOfTroops()
                  << std::endl;
        std::cout << "player2's " << pCountry2->getCountryName() << " now has " << pCountry2->getNumberOfTroops()
                  << std::endl;
        std::cout << "\033[31m";
    }

    return success;
}

bool test_Player_fortify(bool verbose = false) {

    bool success = true;
    const int numArmies = 4;
    const int numFortify = 10;

    std::vector<Map::Country *> ownedCountries1 = std::vector<Map::Country *>();

    Map::Country country1 = Map::Country(0, "country1", 1);
    country1.setPlayerOwnerID(1);
    country1.setNumberOfTroops(numArmies);

    Map::Country *pCountry1 = &country1;
    ownedCountries1.push_back(pCountry1);

    Player player1 = Player(ownedCountries1, Hand(), DiceRoller(), 1);

    if (player1.fortify(pCountry1, numFortify) == PlayerAction::FAILED) {
        success = false;
    }

    if (verbose) {
        std::cout << "\033[35m";
        std::cout << "player1's " << country1.getCountryName() << " had " << numArmies << " armies" << std::endl;
        std::cout << "player1's " << country1.getCountryName() << " now has " << country1.getNumberOfTroops()
                  << " armies" << std::endl;
        std::cout << "\033[31m";
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

    std::cout << assert("Player", "getHand", test_Player_getHand(true)) << std::endl;

    std::cout << assert("Player", "getDiceRoller", test_Player_getDiceRoller(true)) << std::endl;

    std::cout << assert("Player", "attack", test_Player_attack(true)) << std::endl;

    std::cout << assert("Player", "reinforce", test_Player_reinforce(true)) << std::endl;

    std::cout << assert("Player", "fortify", test_Player_fortify(true)) << std::endl;

}