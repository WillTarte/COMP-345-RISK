//
// Created by William on 9/21/2019.
//

#include "../include/Player.h"
#include "../include/Map.h"
#include <iostream>
#include <utility>
#include <GameEngine.h>

/**
 * Player constructor
 *
 * @param ownedCountries vector of Map::Country pointers
 * @param cards Hand object
 * @param diceRoller DiceRoller object
 * @param playerId this Player' integer id
 */
Player::Player(std::vector<Map::Country*> ownedCountries, Hand cards, DiceRoller diceRoller, const int playerId) {
    pOwnedCountries = new std::vector<Map::Country*>(std::move(ownedCountries)); // avoid unnecessary copy
    pCards = new Hand(cards);
    pDiceRoller = new DiceRoller(diceRoller);
    pPlayerId = new int(playerId);
}

/**
 * Checks if the number of attacking and defending dice are valid
 *
 * @param numAttackingDice the number of attacking dice to roll
 * @param numDefendingDice the number of defending dice to roll
 * @param attackingArmies the number of armies on the attacking country
 * @param defendingArmies the number of armies on the defending country
 * @return true if the numbers of dice are valid
 */
static bool checkValidDiceNumbers(const int numAttackingDice, const int numDefendingDice, const int attackingArmies,
                                  const int defendingArmies) {
    return numAttackingDice < attackingArmies || numAttackingDice > 0 || numAttackingDice <= 3
           || numDefendingDice <= defendingArmies || numDefendingDice > 0 || numDefendingDice <= 2;
}

/**
 * Exchanges a countries owner with a new owner
 *
 * @param attackingPlayer the player who won the country
 * @param defendingPlayer the player who lost the country
 * @param country the country to exchange ownership of
 * @return true if the exchange succeeded
 */
static bool exchangeCountryOwnership(Player& attackingPlayer, Player* defendingPlayer, Map::Country& country) {

    country.setPlayerOwnerID(attackingPlayer.getPlayerId());
    for (unsigned long i = 0; i < defendingPlayer->getOwnedCountries()->size(); i++) {
        if (defendingPlayer->getOwnedCountries()->at(i)->getCountryName() == country.getCountryName()) {
            attackingPlayer.getOwnedCountries()->push_back(defendingPlayer->getOwnedCountries()->at(i));
            defendingPlayer->getOwnedCountries()->erase(defendingPlayer->getOwnedCountries()->begin() + i);
            return true;
        }
    }
    return false;
}

/**
 * Helper method to print a country's info + its neighbours
 *
 * @param countries vector of countries
 */
static void showCountries(std::vector<Map::Country*> countries) {
    for (auto& ct: countries) {
        std::cout << "You own: " << ct->getCountryName() << " with " << ct->getNumberOfTroops() << " armies." << std::endl;
        std::cout << "\tNeighbours: " << std::endl;
        for(auto& nb: *ct->getAdjCountries()) {
            std::cout << "\t\tName: " << nb->getCountryName() << " Armies: " << nb->getNumberOfTroops() << " Owned by: Player " << nb->getPlayerOwnerID() << std::endl;
        }
    }
}

/**
 * Private helper Method to execute the logic of the Player attack() behavior.
 *
 * @param fromCountry the country to attack from
 * @param toCountry   the country to attack
 * @param defendingPlayer the defending player (owns toCountry)
 * @param numAttackingDice number of attacking dice
 * @param numDefendingDice number of defending dice
 * @return integer representing if the attack succeeded or not
 */
int Player::executeAttack(Map::Country* fromCountry, Map::Country* toCountry, Player* defendingPlayer, int numAttackingDice, int numDefendingDice) {
    /*
     * 1. The attacking player rolls 1-3 dice, having +1 army than dice rolled. 1 dice roll per attacking army.
     * 2. Defender rolls 2 dice, 1 for each army defending
     * 3. Compare highest dice rolls per army, defender wins ties. lower number loses army
     * 4. If the defending country loses all its armies, the attacking country has to place
     * at least 1 of the attacking armies on the conquered country, up to all but one army
     */

    //TODO - implement the attack method and fix the driver

    // Roll
    std::vector<int> attackingRolls = this->getDiceRoller().roll(numAttackingDice);
    std::vector<int> defendingRolls = defendingPlayer->getDiceRoller().roll(numDefendingDice);

    // Compare the rolls, decide who wins and change number of armies of each country. Maybe update if attacking wins
    while (!attackingRolls.empty() && !defendingRolls.empty()) {
        int aRoll = attackingRolls.back();
        int dRoll = defendingRolls.back();
        attackingRolls.pop_back();
        defendingRolls.pop_back();
        std::cout << "\nAttacker rolled a " << aRoll << ". Defender rolled a " << dRoll << "." << std::endl;
        if (aRoll > dRoll) {
            // defender loses 1 army
            std::cout << "\n[DEFENDER] Defender lost 1 army!" << std::endl;
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() - 1);
            // then, if no armies left on defending country, attacking player conquers it
            if (toCountry->getNumberOfTroops() == 0) {
                std::cout << "\n[DEFENDER] Defender has lost possession of country " << toCountry->getCountryName()
                          << std::endl;
                if (!exchangeCountryOwnership(*this, defendingPlayer, *toCountry)) {
                    return PlayerAction::FAILED;
                } else {
                    int newArmies = 0;
                    do {
                        cout << "\n[ATTACKER] How many armies do you want to place on your new country?(1 to "
                             << fromCountry->getNumberOfTroops() - 1 << ")";
                        cin >> newArmies;
                        if (newArmies > fromCountry->getNumberOfTroops() - 1 || newArmies < 1) {
                            cout << "\nInvalid number! Try again." << std::endl;
                            continue;
                        } else {
                            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - newArmies);
                            toCountry->setNumberOfTroops(newArmies);
                            break;
                        }
                    } while (newArmies > fromCountry->getNumberOfTroops() - 1 || newArmies < 1);
                    return PlayerAction::SUCCEEDED;
                }
            }
        } else {
            // attacker loses 1 army
            std::cout << "\nAttacker lost 1 army!";
            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - 1);
        }
    }
    return PlayerAction::SUCCEEDED;
}

/**
 * Once per turn, this player can place a number of armies on one of his/her country
 * @returns An integer representing the success/failure of the action
 */
int Player::reinforce() {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade at least 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */
//TODO - implement the reinforce method and fix the driver
    /*
    Map::Country& countryToFortify, const int numArmies

    if (!checkOwnedByPlayer(*this, countryToReinforce)) {
        return PlayerAction::FAILED;
    }

    countryToReinforce.setNumberOfTroops(countryToReinforce.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << countryToReinforce.getPlayerOwnerID() << " has fortified "
              << countryToReinforce.getCountryName() << std::endl;

    return PlayerAction::SUCCEEDED;
    */

    return 0;
}

/**
 * The act of moving armies between adjacent and owned countries
 * @returns An integer representing the success/failure of the action
 */
int Player::fortify() {
    /*
     * Act of moving armies between this player's owned countries.
     * fromCountry and toCountry have to be owned by this player and adjacent to each other.
     */

    //TODO - implement the fortify method and fix the driver
    /*
    if (numArmies >= fromCountry.getNumberOfTroops()) {
        return PlayerAction::FAILED;
    }

    if (!checkIfAdjacent(fromCountry, toCountry) || !checkOwnedByPlayer(*this, fromCountry) ||
        !checkOwnedByPlayer(*this, toCountry)) {
        return PlayerAction::FAILED;
    }

    fromCountry.setNumberOfTroops(fromCountry.getNumberOfTroops() - numArmies);
    toCountry.setNumberOfTroops(toCountry.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << this->getPlayerId() << " has reinforced " << toCountry.getCountryName() << " from "
              << fromCountry.getCountryName() << std::endl;
    return PlayerAction::SUCCEEDED;
     */

    return 0;
}

/**
 * Encapsulates the whole business code of the Player's attack behavior
 *
 * @return integer representing if the attack succeeded or not
 */
int Player::attack() {

    char playerChoice = 0;
    int fromCountryIndex = -1;
    int toCountryIndex = -1;
    int numAttackingDice = -1;
    int numDefendingDice = -1;

    /* USER DECISION TO ATTACK OR NOT*/
    do {
        std::cin.clear();
        std::cout << "It is Player " << this->getPlayerId() << "'s turn to attack!" << std::endl;
        std::cout << "\n[ATTACKER] Will you attack?(y/n)";
        std::cin >> playerChoice;
        if (playerChoice != 'y' && playerChoice != 'n') {
            std::cout << "\nInvalid Input. Please try again.\n";
            continue;
        }
        /*---------------------------------------------*/

        if (playerChoice == 'y') {
            /*USER CHOOSES FROM WHICH COUNTRY TO ATTACK*/
            do {
                std::cin.clear();
                showCountries(*this->pOwnedCountries);
                std::cout << "\n[ATTACKER] From which country do you want to attack?(choose 0 to "
                          << this->getOwnedCountries()->size() - 1 << ")";
                std::cin >> fromCountryIndex;
                if (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries()->size() - 1) {
                    std::cout << "\nInvalid Input. Please try again.\n";
                    continue;
                }
            } while (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries()->size() - 1);
            /*---------------------------------------------*/
        } else {
            return PlayerAction::ABORTED;
        }

        // Get the country to attack from
        Map::Country* fromCountry = this->getOwnedCountries()->at(fromCountryIndex);
        if (fromCountry->getNumberOfTroops() < 2) {
            std::cout << "Country " << fromCountry->getCountryName()
                      << " does not have enough armies to attack. Choose a different country. Proceeding...\n";
            this->attack();
        }

        /* USER CHOOSES WHICH COUNTRY TO ATTACK*/
        do {
            std::cout << "\nCountry " << fromCountry->getCountryName() << " has "
                      << fromCountry->getAdjCountries()->size()
                      << " neighbour(s):\n";
            for (auto& nb: *fromCountry->getAdjCountries()) {
                std::cout << "\t\tName: " << nb->getCountryName() << " Armies: " << nb->getNumberOfTroops()
                          << " Owned by: Player " << nb->getPlayerOwnerID() << std::endl;
            }
            std::cout << "\n[ATTACKER] Which country would you like to attack?(0 to "
                      << fromCountry->getAdjCountries()->size() - 1 << ")";
            std::cin >> toCountryIndex;
            if (toCountryIndex < 0 || toCountryIndex > (int) fromCountry->getAdjCountries()->size() - 1) {
                std::cout << "\nInvalid Input. Please try again.\n";
                continue;
            }
        } while (toCountryIndex < 0 || toCountryIndex > (int) fromCountry->getAdjCountries()->size() - 1);
        /*---------------------------------------------*/

        Map::Country* toCountry = fromCountry->getAdjCountries()->at(toCountryIndex);
        if (fromCountry->getPlayerOwnerID() == toCountry->getPlayerOwnerID()) {
            std::cout << "\n[ATTACKER] You cannot attack your own country! Proceeding...\n\n";
            continue;
        }

        /*USER CHOOSES HOW MANY DICE TO ROLL*/
        do {
            std::cout << "[ATTACKER] How many dice will Player " << this->getPlayerId() << " roll?(1 to ";
            if (fromCountry->getNumberOfTroops() > 3) {
                std::cout << "3)";
            } else {
                std::cout << fromCountry->getNumberOfTroops() - 1 << ")";
            }
            std::cin >> numAttackingDice;
            if (numAttackingDice < 1 || numAttackingDice > fromCountry->getNumberOfTroops() - 1 ||
                numAttackingDice > 3) {
                std::cout << "\nInvalid Input. Please try again.\n";
                continue;
            }
        } while (numAttackingDice < 1 || numAttackingDice > fromCountry->getNumberOfTroops() - 1 ||
                 numAttackingDice > 3);
        /*---------------------------------------------*/

        /*USER CHOOSES HOW MANY DICE TO ROLL*/
        do {
            std::cout << "[DEFENDER] How many dice will Player " << toCountry->getPlayerOwnerID() << " roll?(1 to ";
            if (toCountry->getNumberOfTroops() >= 2) {
                std::cout << "2)";
            } else {
                std::cout << "1)";
            }
            std::cin >> numDefendingDice;
            if (numDefendingDice < 1 || numDefendingDice > 2 || numDefendingDice > fromCountry->getNumberOfTroops()) {
                std::cout << "\nInvalid Input. Please try again.\n";
                continue;
            }
        } while (numDefendingDice < 1 || numDefendingDice > 2 || numDefendingDice > fromCountry->getNumberOfTroops());
        /*---------------------------------------------*/

        /* GET THE DEFENDING PLAYER */
        Player* defendingPlayer = nullptr;
        for (unsigned long i = 0; i < GameLoop::getInstance()->getAllPlayers().size(); i++) {
            if (GameLoop::getInstance()->getAllPlayers().at(i)->getPlayerId() == toCountry->getPlayerOwnerID()) {
                defendingPlayer = GameLoop::getInstance()->getAllPlayers().at(i);
                break;
            }
        }
        if (defendingPlayer == nullptr) {
            return PlayerAction::FAILED;
        }
        /*---------------------------------------------*/

        if (this->executeAttack(fromCountry, toCountry, defendingPlayer, numAttackingDice, numDefendingDice) ==
            PlayerAction::SUCCEEDED) {
            cout << "\n[ATTACK SUCCEEDED] - Proceeding." << std::endl;
            continue;
        } else {
            cout << "\n[ATTACK FAILED] - Make sure that you chose appropriate values." << std::endl;
            continue;
        }
    } while (true);
}

