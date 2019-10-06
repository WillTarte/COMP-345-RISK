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
int
Player::attack(Map::Country *fromCountry, Map::Country *toCountry, Player *defendingPlayer, const int numAttackingDice,
               const int numDefendingDice) {
    /*
     * 1. The attacking player rolls 1-3 dice, having +1 army than dice rolled. 1 dice roll per attacking army.
     * 2. Defender rolls 2 dice, 1 for each army defending
     * 3. Compare highest dice rolls per army, defender wins ties. lower number loses army
     * 4. If the defending country loses all its armies, the attacking country has to place
     * at least 1 of the attacking armies on the conquered country, up to all but one army
     */

    bool valid1 = false;
    bool valid2 = false;

    // Check if passed countries are valid (adjacent to each other, fromCountry owned by this player, toCountry owned by defender)
    for (unsigned int i = 0; i < this->getOwnedCountries().size(); i++) {

        // check if fromCountry is valid
        if (this->getOwnedCountries().at(i)->getCountryName() == fromCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == fromCountry->getPlayerOwnerID()) {
            valid1 = true;
        }
    }

    for (unsigned int i = 0; i < defendingPlayer->getOwnedCountries().size(); i++) {

        // check if toCountry is valid
        if (defendingPlayer->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()
            && defendingPlayer->getOwnedCountries().at(i)->getPlayerOwnerID() == toCountry->getPlayerOwnerID()) {
            valid2 = true;
        }
    }

    if (!valid1 || !valid2) {
        return PlayerAction::FAILED;
    }

    // Check if numAttackingDice is valid
    if ((numAttackingDice < fromCountry->getNumberOfTroops() && 0 < numAttackingDice && numAttackingDice <= 3) ||
        (numDefendingDice < toCountry->getNumberOfTroops() && 0 <= numDefendingDice && numDefendingDice <= 2)) {
        return PlayerAction::FAILED;
    }

    // Roll
    std::vector<int> attackingRolls = this->getDiceRoller().roll(numAttackingDice);
    std::vector<int> defendingRolls = defendingPlayer->getDiceRoller().roll(numAttackingDice);

    // Compare the rolls, decide who wins and change number of armies of each country. Maybe update if attacking wins
    while (attackingRolls.empty() || defendingRolls.empty()) {

        int aRoll = attackingRolls.back();
        int dRoll = defendingRolls.back();

        attackingRolls.pop_back();
        defendingRolls.pop_back();

        if (aRoll > dRoll) {

            // defender loses 1 army
            std::cout << "Defender lost 1 army!" << std::endl;
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() - 1);
            // then, if no armies left on defending country, attacking player conquers it

            if (toCountry->getNumberOfTroops() == 0) {
                std::cout << "Defender has lost possession of country " << toCountry->getCountryName() << std::endl;
                toCountry->setPlayerOwnerID(this->getPlayerId());

                for (unsigned int i = 0; i < defendingPlayer->getOwnedCountries().size(); i++) {
                    if (defendingPlayer->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()) {
                        this->getOwnedCountries().push_back(defendingPlayer->getOwnedCountries().at(i));
                        defendingPlayer->getOwnedCountries().erase(defendingPlayer->getOwnedCountries().begin() + i);
                    }
                }
            }
        } else {

            // attacker loses 1 army
            std::cout << "Attacker lost 1 army!" << std::endl;
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
int Player::fortify(Map::Country *countryToFortify, const int numArmies) {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade at least 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */
    bool valid1 = false;
    for (unsigned int i = 0; i < this->getOwnedCountries().size(); i++) {
        if (this->getOwnedCountries().at(i)->getCountryName() == countryToFortify->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == countryToFortify->getPlayerOwnerID() ==
               this->getPlayerId()) {
            countryToFortify->setNumberOfTroops(countryToFortify->getNumberOfTroops() + numArmies);
            valid1 = true;
            break;
        }
    }

    if (!valid1) {
        return PlayerAction::FAILED;
    }

    std::cout << "Player " << countryToFortify->getPlayerOwnerID() << " has fortified "
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
int Player::reinforce(Map::Country *fromCountry, Map::Country *toCountry, const int numArmies) {
    /*
     * Act of moving armies between this player's owned countries.
     * fromCountry and toCountry have to be owned by this player and adjacent to each other.
     */
    bool valid1 = false;
    bool valid2 = false;
    bool valid3 = false;

    if (numArmies >= fromCountry->getNumberOfTroops()) {
        return PlayerAction::FAILED;
    }

    // Check if passed countries are valid (adjacent to each other, owned by this player)
    for (unsigned int i = 0; i < this->getOwnedCountries().size(); i++) {
        // check if fromCountry is valid
        if (this->getOwnedCountries().at(i)->getCountryName() == fromCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == fromCountry->getPlayerOwnerID() ==
               this->getPlayerId()) {
            valid1 = true;
        }
        if (this->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == toCountry->getPlayerOwnerID() ==
               this->getPlayerId()) {
            valid2 = true;
        }
    }

    if (!valid1 || !valid2) {
        return PlayerAction::FAILED;
    }

    // Check to make sure both
    for (unsigned long i = 0; i < fromCountry->pAdjCountries->size(); i++) {
        if (fromCountry->pAdjCountries->at(i)->getCountryName() == toCountry->getCountryName()) {
            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - numArmies);
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() + numArmies);
            valid3 = true;
            break;
        }
    }

    if (!valid3) {
        return PlayerAction::FAILED;
    }

    std::cout << "Player " << this->getPlayerId() << " has reinforced " << toCountry->getCountryName() << std::endl;
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
Player::Player(std::vector<Map::Country *> ownedCountries, Hand cards, DiceRoller diceRoller,
               const int playerId) {
    ownedCountriesPointer = new std::vector<Map::Country *>(std::move(ownedCountries)); // avoid unnecessary copy
    cardsPointer = new Hand(cards);
    diceRollerPointer = new DiceRoller(diceRoller);
    playerIdPointer = new int(playerId);
}



