//
// Created by Willi on 9/21/2019.
//

#include "../include/Player.h"
#include "../include/Map.h"
#include <iostream>
#include <utility>

/**
 * Makes this player attack another player's country
 *
 * @param fromCountry       the country to attack from
 * @param toCountry         the country to attack
 * @param defendingPlayer   the defending player
 * @param numAttackingDice  number of dice the attacking player should roll
 * @param numDefendingDice  number of dice the defending player should roll
 */
int Player::attack(Map::Country* fromCountry, Map::Country* toCountry, Player* defendingPlayer, const int numAttackingDice, const int numDefendingDice) {
    /*
     * 1. The attacking player rolls 1-3 dice, having +1 army than dice rolled. 1 dice roll per attacking army.
     * 2. Defender rolls 2 dice, 1 for each army defending
     * 3. Compare highest dice rolls per army, defender wins ties. lower number loses army
     * 4. If the defending country loses all its armies, the attacking country has to place
     * at least 1 of the attacking armies on the conquered country, up to all but one army
     */

    bool fromCountryValid = false;
    bool toCountryValid = false;

    // Check if passed countries are valid (adjacent to each other, fromCountry owned by this player, toCountry owned by defender)
    for (auto & i : this->getOwnedCountries()) {
        // check if fromCountry is valid
        if (i->getCountryName() == fromCountry->getCountryName()
            && i->getPlayerOwnerID() == fromCountry->getPlayerOwnerID()) {
            fromCountryValid = true;
        }
    }

    for (auto & i : defendingPlayer->getOwnedCountries()) {
        // check if toCountry is valid
        if (i->getCountryName() == toCountry->getCountryName()
            && i->getPlayerOwnerID() == toCountry->getPlayerOwnerID()) {
            toCountryValid = true;
        }
    }

    if (!fromCountryValid || !toCountryValid) {
        return PlayerAction::FAILED;
    }

    // Check if numAttackingDice and numDefendingDice is valid
    if ((numAttackingDice >= fromCountry->getNumberOfTroops() || numAttackingDice <= 0 || numAttackingDice > 3)
        || (numDefendingDice > toCountry->getNumberOfTroops() || numDefendingDice <= 0 || numDefendingDice > 2)) {
        return PlayerAction::FAILED;
    }

    // Roll
    std::vector<int> attackingRolls = this->getDiceRoller().roll(numAttackingDice);
    std::vector<int> defendingRolls = defendingPlayer->getDiceRoller().roll(numDefendingDice);

    // Compare the rolls, decide who wins and change number of armies of each country. Maybe update if attacking wins
    while (!attackingRolls.empty() && !defendingRolls.empty()) {

        int aRoll = attackingRolls.back();
        int dRoll = defendingRolls.back();

        attackingRolls.pop_back();
        defendingRolls.pop_back();

        if (aRoll > dRoll) {
            // defender loses 1 army
            std::cout << "\nDefender lost 1 army!" << std::endl;
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() - 1);

            // then, if no armies left on defending country, attacking player conquers it
            if (toCountry->getNumberOfTroops() == 0) {
                std::cout << "Defender has lost possession of country " << toCountry->getCountryName();
                toCountry->setPlayerOwnerID(this->getPlayerId());

                for (unsigned long i = 0; i < defendingPlayer->getOwnedCountries().size(); i++) {

                    if (defendingPlayer->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()) {
                        this->getOwnedCountries().push_back(defendingPlayer->getOwnedCountries().at(i));
                        defendingPlayer->getOwnedCountries().erase(defendingPlayer->getOwnedCountries().begin() + i);
                        break;
                    }
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
 *
 * @param countryToFortify the country the player wishes to fortify
 * @param numArmies the number of armies to fortify with
 */
int Player::fortify(Map::Country* countryToFortify, const int numArmies) {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade at least 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */

    bool countryToFortifyValid = false;

    for (auto & i : this->getOwnedCountries()) {
        if (i->getCountryName() == countryToFortify->getCountryName()
            && i->getPlayerOwnerID() == countryToFortify->getPlayerOwnerID()
            && countryToFortify->getPlayerOwnerID() == this->getPlayerId()) {

            countryToFortify->setNumberOfTroops(countryToFortify->getNumberOfTroops() + numArmies);
            countryToFortifyValid = true;
            break;
        }
    }

    if (!countryToFortifyValid) {
        return PlayerAction::FAILED;
    }

    std::cout << "\nPlayer " << countryToFortify->getPlayerOwnerID() << " has fortified "
              << countryToFortify->getCountryName() << std::endl;
    return PlayerAction::SUCCEEDED;
}

/**
 * The act of moving armies between adjacent and owned countries
 *
 * @param fromCountry the country to take armies from
 * @param toCountry  the country to move those armies to
 * @param numArmies  the number of armies to move
 */
int Player::reinforce(Map::Country* fromCountry, Map::Country* toCountry, const int numArmies) {
    /*
     * Act of moving armies between this player's owned countries.
     * fromCountry and toCountry have to be owned by this player and adjacent to each other.
     */
    bool fromCountryValid = false;
    bool toCountryValid = false;
    bool reinforceValid = false;

    if (numArmies >= fromCountry->getNumberOfTroops()) {
        return PlayerAction::FAILED;
    }

    // Check if passed countries are valid (adjacent to each other, owned by this player)
    for (auto & i : this->getOwnedCountries()) {
        // check if fromCountry is valid
        if (i->getCountryName() == fromCountry->getCountryName()
            && i->getPlayerOwnerID() == fromCountry->getPlayerOwnerID()
            && fromCountry->getPlayerOwnerID() == this->getPlayerId()) {
            fromCountryValid = true;
        }
        if (i->getCountryName() == toCountry->getCountryName()
            && i->getPlayerOwnerID() == toCountry->getPlayerOwnerID()
            && toCountry->getPlayerOwnerID() == this->getPlayerId()) {
            toCountryValid = true;
        }
    }

    if (!fromCountryValid || !toCountryValid) {
        return PlayerAction::FAILED;
    }

    // Check to make sure both countries are adjacent, and move armies
    for (unsigned long i = 0; i < fromCountry->pAdjCountries->size(); i++) {
        if (fromCountry->pAdjCountries->at(i)->getCountryName() == toCountry->getCountryName()) {
            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - numArmies);
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() + numArmies);
            reinforceValid = true;
            break;
        }
    }

    if (!reinforceValid) {
        return PlayerAction::FAILED;
    }

    std::cout << "\nPlayer " << this->getPlayerId() << " has reinforced " << toCountry->getCountryName() << " from "
              << fromCountry->getCountryName() << std::endl;
    return PlayerAction::SUCCEEDED;
}

/**
 * Player constructor
 *
 * @param ownedCountries vector of Map::Country pointers
 * @param cards Hand object
 * @param diceRoller DiceRoller object
 * @param playerId this Player' integer id
 */
Player::Player(std::vector<Map::Country *> ownedCountries, Hand cards, DiceRoller diceRoller, const int playerId) {
    ownedCountriesPointer = new std::vector<Map::Country*>(std::move(ownedCountries)); // avoid unnecessary copy
    cardsPointer = new Hand(cards);
    diceRollerPointer = new DiceRoller(diceRoller);
    playerIdPointer = new int(playerId);
}



