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
Player::Player(std::vector<Map::Country *> ownedCountries, Hand cards, DiceRoller diceRoller, const int playerId) {
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
static bool checkValidDiceNumbers(const int numAttackingDice, const int numDefendingDice, const int attackingArmies, const int defendingArmies) {
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
 * \brief Makes this player attack another player's country.
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
        || !checkIfAdjacent(fromCountry, toCountry) ) {
        return PlayerAction::FAILED;
    }

    // Check if numAttackingDice and numDefendingDice is valid
    if (!checkValidDiceNumbers(numAttackingDice, numDefendingDice, fromCountry.getNumberOfTroops(), toCountry.getNumberOfTroops())) {
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
                if(!exchangeCountryOwnership(*this, defendingPlayer, toCountry)) {
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
 * @param countryToFortify the country the player wishes to fortify
 * @param numArmies the number of armies to fortify with
 */
int Player::fortify(Map::Country& countryToFortify, const int numArmies) {
    /* Act of collecting new armies and placing them on the map
     * 0. At the start of your turn, if you have 5+ cards, player must trade at least 1 set.
     * 1. Trade valid sets of cards to receive armies
     * 2. Place received armies on the map
     */

    if (!checkOwnedByPlayer(*this, countryToFortify)) {
        return PlayerAction::FAILED;
    }

    countryToFortify.setNumberOfTroops(countryToFortify.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << countryToFortify.getPlayerOwnerID() << " has fortified "
              << countryToFortify.getCountryName() << std::endl;

    return PlayerAction::SUCCEEDED;
}

/**
 * The act of moving armies between adjacent and owned countries
 *
 * @param fromCountry the country to take armies from
 * @param toCountry  the country to move those armies to
 * @param numArmies  the number of armies to move
 */
int Player::reinforce(Map::Country& fromCountry, Map::Country& toCountry, const int numArmies) {
    /*
     * Act of moving armies between this player's owned countries.
     * fromCountry and toCountry have to be owned by this player and adjacent to each other.
     */

    if (numArmies >= fromCountry.getNumberOfTroops()) {
        return PlayerAction::FAILED;
    }

    if(!checkIfAdjacent(fromCountry, toCountry) || !checkOwnedByPlayer(*this, fromCountry) || !checkOwnedByPlayer(*this, toCountry) ) {
        return PlayerAction::FAILED;
    }

    fromCountry.setNumberOfTroops(fromCountry.getNumberOfTroops() - numArmies);
    toCountry.setNumberOfTroops(toCountry.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << this->getPlayerId() << " has reinforced " << toCountry.getCountryName() << " from "
              << fromCountry.getCountryName() << std::endl;
    return PlayerAction::SUCCEEDED;
}