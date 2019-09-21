//
// Created by Willi on 9/21/2019.
//

#include "../include/Player.h"
#include "../include/Map.h"

void Player::attack(Map::Country *fromCountry, Map::Country *toCountry, const Player *defendingPlayer) {
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
    for (int i = 0; i < this->getOwnedCountries().size(); i++) {
        // check if fromCountry is valid
        if (this->getOwnedCountries().at(i)->getCountryName() == fromCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == fromCountry->getPlayerOwnerID()) {
            valid1 = true;
        }
        if (this->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() != toCountry->getPlayerOwnerID()) {
            valid2 = true;
        }
    }

    if (!valid1 || !valid2) {
        return;
    }
    // roll

}

void Player::fortify(Map::Country *countryToFortify, const int numArmies) {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade atleast 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */
    for (int i = 0; i < this->getOwnedCountries().size(); i++) {
        if (this->getOwnedCountries().at(i)->getCountryName() == countryToFortify->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == countryToFortify->getPlayerOwnerID()) {
            countryToFortify->setNumberOfTroops(countryToFortify->getNumberOfTroops() + numArmies);
            break;
        }
    }
    return;
}

void Player::reinforce(Map::Country *fromCountry, Map::Country *toCountry, const int numArmies) {

    bool valid1 = false;
    bool valid2 = false;

    // Check if passed countries are valid (adjacent to each other, owned by this player)
    for (int i = 0; i < this->getOwnedCountries().size(); i++) {
        // check if fromCountry is valid
        if (this->getOwnedCountries().at(i)->getCountryName() == fromCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == fromCountry->getPlayerOwnerID()) {
            valid1 = true;
        }
        if (this->getOwnedCountries().at(i)->getCountryName() == toCountry->getCountryName()
            && this->getOwnedCountries().at(i)->getPlayerOwnerID() == toCountry->getPlayerOwnerID()) {
            valid2 = true;
        }
    }

    if (!valid1 || !valid2) {
        return;
    }

    for (int i = 0; i < fromCountry->pAdjCountries->size(); i++) {
        if (fromCountry->pAdjCountries->at(i)->getCountryName() == toCountry->getCountryName()) {
            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - numArmies);
            toCountry->setNumberOfTroops(toCountry->getNumberOfTroops() + numArmies);
            break;
        }
    }
    return;
}

