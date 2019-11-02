//
// Created by William on 9/21/2019.
//

#include "../include/Player.h"
#include "../include/Map.h"
#include <iostream>
#include <utility>

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
 * Checks if the passed country is owned by the passed player
 *
 * @param player the player
 * @param country the country
 * @return true if the country is owned by the player
 */
static bool checkOwnedByPlayer(Player& player, Map::Country& country) {
    return player.getPlayerId() == country.getPlayerOwnerID();
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
static bool exchangeCountryOwnership(Player& attackingPlayer, Player& defendingPlayer, Map::Country& country) {
    country.setPlayerOwnerID(attackingPlayer.getPlayerId());
    for (unsigned long i = 0; i < defendingPlayer.getOwnedCountries().size(); i++) {
        if (defendingPlayer.getOwnedCountries().at(i)->getCountryName() == country.getCountryName()) {
            attackingPlayer.getOwnedCountries().push_back(defendingPlayer.getOwnedCountries().at(i));
            defendingPlayer.getOwnedCountries().erase(defendingPlayer.getOwnedCountries().begin() + i);
            return true;
        }
    }
    return false;
}

/**
 * Makes this player attack another player's country.
 * If the defending country has no armies left, places 1 army of the attacking country on the newly acquired country.
 * @param fromCountry       the country to attack from
 * @param toCountry         the country to attack
 * @param defendingPlayer   the defending player
 * @param numAttackingDice  number of dice the attacking player should roll
 * @param numDefendingDice  number of dice the defending player should roll
 */
int Player::attack(Map::Country& fromCountry, Map::Country& toCountry, Player& defendingPlayer, const int numAttackingDice, const int numDefendingDice) {
    /*
     * 1. The attacking player rolls 1-3 dice, having +1 army than dice rolled. 1 dice roll per attacking army.
     * 2. Defender rolls 2 dice, 1 for each army defending
     * 3. Compare highest dice rolls per army, defender wins ties. lower number loses army
     * 4. If the defending country loses all its armies, the attacking country has to place
     * at least 1 of the attacking armies on the conquered country, up to all but one army
     */

    // Check if passed countries are valid (adjacent to each other, fromCountry owned by this player, toCountry owned by defender)
    if (!checkOwnedByPlayer(*this, fromCountry)
        || !checkOwnedByPlayer(defendingPlayer, toCountry)
        || !checkIfAdjacent(fromCountry, toCountry)) {
        return PlayerAction::FAILED;
    }

    // Check if numAttackingDice and numDefendingDice is valid
    if (!checkValidDiceNumbers(numAttackingDice, numDefendingDice, fromCountry.getNumberOfTroops(),
                               toCountry.getNumberOfTroops())) {
        return PlayerAction::FAILED;
    }

    // Roll
    std::vector<int> attackingRolls = this->getDiceRoller().roll(numAttackingDice);
    std::vector<int> defendingRolls = defendingPlayer.getDiceRoller().roll(numDefendingDice);

    // Compare the rolls, decide who wins and change number of armies of each country. Maybe update if attacking wins
    while (!attackingRolls.empty() && !defendingRolls.empty()) {
        int aRoll = attackingRolls.back();
        int dRoll = defendingRolls.back();
        attackingRolls.pop_back();
        defendingRolls.pop_back();

        if (aRoll > dRoll) {
            // defender loses 1 army
            std::cout << "\nDefender lost 1 army!" << std::endl;
            toCountry.setNumberOfTroops(toCountry.getNumberOfTroops() - 1);
            // then, if no armies left on defending country, attacking player conquers it
            if (toCountry.getNumberOfTroops() == 0) {
                std::cout << "Defender has lost possession of country " << toCountry.getCountryName();
                if (!exchangeCountryOwnership(*this, defendingPlayer, toCountry)) {
                    return PlayerAction::FAILED;
                } else {
                    return PlayerAction::SUCCEEDED;
                }
            }
        } else {
            // attacker loses 1 army
            std::cout << "\nAttacker lost 1 army!";
            fromCountry.setNumberOfTroops(fromCountry.getNumberOfTroops() - 1);
        }
    }
    return PlayerAction::SUCCEEDED;
}

/**
 * Once per turn, this player can place a number of armies on one of his/her country
 *
 * @param countryToReinforce the country the player wishes to fortify
 * @param numArmies the number of armies to fortify with
 */
int Player::reinforce(Map::Country& countryToReinforce, int numArmies) {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade at least 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */

    if (!checkOwnedByPlayer(*this, countryToReinforce)) {
        return PlayerAction::FAILED;
    }

    countryToReinforce.setNumberOfTroops(countryToReinforce.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << countryToReinforce.getPlayerOwnerID() << " has reinforced "
              << countryToReinforce.getCountryName() << std::endl;

    return PlayerAction::SUCCEEDED;
 }


/**
 * The act of moving armies between adjacent and owned countries
 *
 * @param fromCountry the country to take armies from
 * @param countryToFortify  the country the player wants to fortify
 * @param numArmies  the number of armies to move
 */

int Player::fortify(Map::Country& fromCountry, Map::Country& countryToFortify, int numArmies) {
    /*
     * Act of moving armies between this player's owned countries.
     * fromCountry and countryToFortify have to be owned by this player and adjacent to each other.
     */

    if (numArmies >= fromCountry.getNumberOfTroops()) {
        return PlayerAction::FAILED;
    }

    if (!checkIfAdjacent(fromCountry, countryToFortify) || !checkOwnedByPlayer(*this, fromCountry) ||
        !checkOwnedByPlayer(*this, countryToFortify)) {
        return PlayerAction::FAILED;
    }

    fromCountry.setNumberOfTroops(fromCountry.getNumberOfTroops() - numArmies);
    countryToFortify.setNumberOfTroops(countryToFortify.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << this->getPlayerId() << " has fortified " << countryToFortify.getCountryName() << " from "
              << fromCountry.getCountryName() << std::endl;
    return PlayerAction::SUCCEEDED;
}
/*
int Player::attack() {

    char playerChoice = 0;
    int fromCountryIndex = -1;
    int toCountryIndex = -1;
    int numAttackingDice = -1;
    int numDefendingDice = -1;

    std::cout << "It is Player " << this->getPlayerId() << "'s turn to attack!" << std::endl;

    do {
        std::cin.clear();
        std::cout << "\nWill you attack? (y/n)";
        std::cin >> playerChoice;
        if (playerChoice != 'y' && playerChoice != 'n') {
            std::cout << "\nInvalid input. Please try again.\n";
            continue;
        }
    } while (playerChoice != 'y' && playerChoice != 'n');

    if (playerChoice == 'y') {
        do {
            std::cin.clear();
            std::cout << "\nYou own " << this->getOwnedCountries().size()
                      << " countries. From which country do you want to attack ? (choose 0 to "
                      << this->getOwnedCountries().size() -1 << ")";
            std::cin >> fromCountryIndex;
            if (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries().size() - 1) {
                std::cout << "\nInvalid input. Please try again.\n";
                continue;
            }
        } while (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries().size() - 1);
    }else {
        return PlayerAction::ABORTED;
    }

    Map::Country* fromCountry = this->getOwnedCountries().at(fromCountryIndex);
    //TODO: print the adj countries' info
    do {
        std::cout << "Country " << fromCountry->getCountryName() << " has " << fromCountry->pAdjCountries->size() << " neighbours\n";
        std::cout << "Which country would you like to attack ? (0 to " << fromCountry->pAdjCountries->size() - 1 << ")";
        std::cin >> toCountryIndex;
        if (toCountryIndex < 0 || toCountryIndex > (int) fromCountry->pAdjCountries->size() - 1) {
            std::cout << "\nInvalid input. Please try again.\n";
            continue;
        }
    } while (toCountryIndex < 0 || toCountryIndex > (int) fromCountry->pAdjCountries->size() - 1);
    Map::Country* toCountry = fromCountry->pAdjCountries->at(toCountryIndex);

    //TODO: somehow have access to all players

    do {
        std::cout << "[ATTACKER] How many dice will Player " << this->getPlayerId() << " roll ? (1 to 2)";
        std::cin >> numAttackingDice;
        if (numAttackingDice < 1 || numAttackingDice > 2) {
            std::cout <<"\nInvalid input. Please try again.\n";
            continue;
        }
    } while (numAttackingDice < 1 || numAttackingDice > 2);

    return this->attack(*fromCountry, *toCountry, (Player&) nullptr, numAttackingDice, numDefendingDice);

}
*/
int Player::reinforce() {return 0;}

int Player::fortify() {

    char playerChoice = 0;
    int fromCountryIndex = -1;
    int ctryToFortIndex = -1;
    int numArmies = -1;

    std::cout << "\nLast phase for Player " << this->getPlayerId() << " (fortification) !" << std::endl;

    do {
        std::cin.clear();
        std::cout << "\nDo you wish to fortify a country this turn (y/n)? ";
        std::cin >> playerChoice;
        if (playerChoice != 'y' && playerChoice != 'n') {
            std::cout << "\nInvalid input. Try again.\n";
            continue;
        }
    } while (playerChoice != 'y' && playerChoice != 'n');

    if (playerChoice == 'y') {
        do {
            std::cin.clear();
            std::cout << "\nYour owned countries: ";
            //Print all the player's owned countries so they are reminded what they have in order to choose
            for (const auto i : getOwnedCountries()){
                std::cout << i->getCountryName() << " ";
            }
            std::cout << "\nYou own " << this->getOwnedCountries().size()
                      << " countries. Which country do you want to move armies from ? (choose 0 to "
                      << this->getOwnedCountries().size() - 1 << " ) ";
            std::cin >> fromCountryIndex;
            if (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries().size() - 1) {
                std::cout << "\nInvalid input. Try again.\n";
                continue;
            }
        } while (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries().size() - 1);
    } else {
        return PlayerAction::ABORTED;
    }

    Map::Country* fromCountry = this->getOwnedCountries().at(fromCountryIndex);

    do{
        std::cout << "\nYou have " << fromCountry->getNumberOfTroops() << " armies in this country."
                  << " How many would you like to move ? (0 to " << fromCountry->getNumberOfTroops() - 1 << ") ";
        std::cin >> numArmies;
        if (numArmies < 0 || numArmies > fromCountry->getNumberOfTroops() - 1) {
            std::cout << "\nInvalid input. Try again.\n";
            continue;
        }
    } while (numArmies < 0 || numArmies > fromCountry->getNumberOfTroops() - 1);

    //not sure, should prob only display adjacent countries that belong to the player
    do{
        std::cout << "\nCountry " << fromCountry->getCountryName() << " has " << fromCountry->pAdjCountries->size()
                  << " neighbours\n";
        std::cout << "Which of your countries would you like to move your armies to ? (0 to " << fromCountry->pAdjCountries->size() - 1 << ") ";
        std::cin >> ctryToFortIndex;
        if (ctryToFortIndex < 0 || ctryToFortIndex > (int) fromCountry->pAdjCountries->size() - 1) {
            std::cout << "\nInvalid input. Try again.\n";
            continue;
        }
    } while (ctryToFortIndex < 0 || ctryToFortIndex > (int) fromCountry->pAdjCountries->size() - 1);
    Map::Country* countryToFortify = fromCountry->pAdjCountries->at(ctryToFortIndex);

    return this->fortify(*fromCountry, *countryToFortify, numArmies);
}
