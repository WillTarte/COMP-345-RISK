//
// Created by Willi on 9/21/2019.
//

#include "../include/Player.h"
#include "../include/Map.h"

/**
 * Makes this player attack another player's country
 *
 * @param fromCountry       the country to attack from
 * @param toCountry         the country to attack
 * @param defendingPlayer   the defending player
 * @param numAttackingDice  number of dice the attacking player should roll
 * @param numDefendingDice  number of dice the defending player should roll
 */
void
Player::attack(Map::Country *fromCountry, Map::Country *toCountry, Player *defendingPlayer, const int numAttackingDice,
               const int numDefendingDice) {
    /* Considering that the country being attacked and the attacking country
     * are adjacent, and that the attacking country as atleast 2 armies. Then
     *
     * 1. The attacking player rolls 1-3 dice, having +1 army than dice rolled. 1 dice roll per attacking army.
     * 2. Defender rolls 2 dice, 1 for each army defending
     * 3. Compare highest dice rolls per army, defender wins ties. lower number loses army
     * 4. If the defending country loses all its armies, the attacking country has to place
     * at least 1 of the attacking armies on the conquered country, up to all but one army
     *
     */

    bool valid1 = false;
    bool valid2 = false;

    // Check if passed countries are valid (adjacent to each other, owned by this player)
    for (unsigned int i = 0; i < this->getOwnedCountries().size(); i++) {
        // check if fromCountry is valid
        if (this->getOwnedCountries().at(i)->getCountryName() == fromCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == fromCountry->getPlayerOwnerID()) {
            valid1 = true;
        }
        // check if toCountry is valid
        if (this->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() != toCountry->getPlayerOwnerID()) {
            valid2 = true;
        }
    }

    if (!valid1 || !valid2) {
        return;
    }

    // Check if numAttackingDice is valid
    if ((numAttackingDice < fromCountry->getNumberOfTroops() && 0 < numAttackingDice && numAttackingDice <= 3) ||
        (numDefendingDice < toCountry->getNumberOfTroops() && 0 <= numDefendingDice && numDefendingDice <= 2)) {
        return;
    }

    // Roll
    std::vector<int> attackingRolls = this->getDiceRoller().roll(numAttackingDice);
    std::vector<int> defendingRolls = defendingPlayer->getDiceRoller().roll(numAttackingDice);

    // Compare the rolls, decide who wins and change number of armies of each country. Maybe update if attacking wins



}

/**
 * Once per turn, this player can place a number of armies on one of his/her country
 *
 * @param countryToFortify the country the player wishes to fortify
 * @param numArmies the number of armies to fortify with
 */
void Player::fortify(Map::Country *countryToFortify, const int numArmies) {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade at least 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */
    for (unsigned int i = 0; i < this->getOwnedCountries().size(); i++) {
        if (this->getOwnedCountries().at(i)->getCountryName() == countryToFortify->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == countryToFortify->getPlayerOwnerID() ==
               this->getPlayerId()) {
            countryToFortify->setNumberOfTroops(countryToFortify->getNumberOfTroops() + numArmies);
            break;
        }
    }
    return;
}

/**
 * The act of moving armies between adjacent and owned countries
 *
 * @param fromCountry the country to take armies from
 * @param toCountry  the country to move those armies to
 * @param numArmies  the number of armies to move
 */
void Player::reinforce(Map::Country *fromCountry, Map::Country *toCountry, const int numArmies) {

    bool valid1 = false;
    bool valid2 = false;

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
        return;
    }

    for (unsigned int i = 0; i < fromCountry->pAdjCountries->size(); i++) {
        if (fromCountry->pAdjCountries->at(i)->getCountryName() == toCountry->getCountryName()) {
            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - numArmies);
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() + numArmies);
            break;
        }
    }
    return;
}

