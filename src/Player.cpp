//
// Created by William on 9/21/2019.
//

#include "../include/Player.h"
#include "../include/Map.h"
#include "../include/Cards.h"
#include "../include/GameEngine.h"
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
Player::Player(std::vector<Map::Country*> ownedCountries, Hand* cards, DiceRoller* diceRoller, const int playerId) {
    pOwnedCountries = new std::vector<Map::Country*>(std::move(ownedCountries)); // avoid unnecessary copy
    pCards = cards;
    pDiceRoller = diceRoller;
    pPlayerId = new int(playerId);
}

/**
 * Player destructor
 */
Player::~Player() {
    delete pOwnedCountries;
    delete pCards;
    delete pDiceRoller;
    delete pPlayerId;
}

/**
 * Player copy constructor
 * @param toCopy
 */
Player::Player(const Player &toCopy) {
    pOwnedCountries = new vector<Map::Country*>;
    pCards = new Hand();
    pDiceRoller = new DiceRoller();
    pPlayerId = new int(*toCopy.pPlayerId);
    *pOwnedCountries = *toCopy.pOwnedCountries;
    *pCards = *toCopy.pCards;
    *pDiceRoller = *toCopy.pDiceRoller;
}

/**
 * Assignment operator for Player class
 * @param rhs the right hand side of the expression
 * @return ???
 */
void Player::operator=(const Player& rhs){
    this->pOwnedCountries = rhs.pOwnedCountries;
    this->pPlayerId = rhs.pPlayerId;
    this->pDiceRoller = rhs.pDiceRoller;
    this->pCards = rhs.pCards;
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
    for (auto& country: countries) {
        std::cout << "You own: " << country->getCountryName() << " with " << country->getNumberOfTroops() << " armies." << std::endl;
        std::cout << "\tNeighbours: " << std::endl;
        for(auto& neighbour: *country->getAdjCountries()) {
            std::cout << "\t\tName: " << neighbour->getCountryName() << " Armies: " << neighbour->getNumberOfTroops() << " Owned by: Player " << neighbour->getPlayerOwnerID() << std::endl;
        }
    }
}

static int getAttackingCountry(Player* attacker) {

    int fromCountryIndex = 0;
    do {
        std::cin.clear();
        showCountries(*attacker->getOwnedCountries());
        std::cout << "\n[ATTACKER] From which country do you want to attack?(choose 0 to "
                  << attacker->getOwnedCountries()->size() - 1 << ")";
        std::cin >> fromCountryIndex;
        if (fromCountryIndex < 0 || fromCountryIndex > (int) attacker->getOwnedCountries()->size() - 1 || cin.fail()) {
            std::cout << "\nInvalid Input. Please try again.\n";
            continue;
        }
    } while (fromCountryIndex < 0 || fromCountryIndex > (int) attacker->getOwnedCountries()->size() - 1 || cin.fail());
    return fromCountryIndex;
}

static bool canExchange(const vector<CardType>& cards) {
    int numInfantry = 0;
    int numArtillery = 0;
    int numCavalry = 0;
    for (const auto i: cards) {
        switch (i) {
            case CardType::INFANTRY:
                numInfantry++;
                break;
            case CardType::CAVALRY:
                numCavalry++;
                break;
            case CardType::ARTILLERY:
                numArtillery++;
                break;
        }
    }
    return (numArtillery >= 3 || numCavalry >= 3 || numInfantry >= 3 ||
            (numInfantry >= 1 && numArtillery >= 1 && numCavalry >= 1));
}

static int getDefendingCountry(Map::Country* fromCountry) {

    int toCountryIndex = 0;
    do {
        std::cin.clear();
        std::cout << "\nCountry " << fromCountry->getCountryName() << " has "
                  << fromCountry->getAdjCountries()->size()
                  << " neighbour(s):\n";
        for (auto& neighbour: *fromCountry->getAdjCountries()) {
            std::cout << "\t\tName: " << neighbour->getCountryName() << " Armies: " << neighbour->getNumberOfTroops()
                      << " Owned by: Player " << neighbour->getPlayerOwnerID() << std::endl;
        }
        std::cout << "\n[ATTACKER] Which country would you like to attack?(0 to "
                  << fromCountry->getAdjCountries()->size() - 1 << ")";
        std::cin >> toCountryIndex;
        if (toCountryIndex < 0 || toCountryIndex > (int) fromCountry->getAdjCountries()->size() - 1 || cin.fail()) {
            std::cout << "\nInvalid Input. Please try again.\n";
            continue;
        }
    } while (toCountryIndex < 0 || toCountryIndex > (int) fromCountry->getAdjCountries()->size() - 1 || cin.fail());
    return toCountryIndex;
}

static int getNumAttackingDice(Player* attacker, Map::Country* fromCountry) {

    int numAttackingDice = 0;
    do {
        std::cin.clear();
        std::cout << "[ATTACKER] How many dice will Player " << attacker->getPlayerId() << " roll?(1 to ";
        if (fromCountry->getNumberOfTroops() > 3) {
            std::cout << "3)";
        } else {
            std::cout << fromCountry->getNumberOfTroops() - 1 << ")";
        }
        std::cin >> numAttackingDice;
        if (numAttackingDice < 1 || numAttackingDice > fromCountry->getNumberOfTroops() - 1 ||
            numAttackingDice > 3 || cin.fail()) {
            std::cout << "\nInvalid Input. Please try again.\n";
            continue;
        }
    } while (numAttackingDice < 1 || numAttackingDice > fromCountry->getNumberOfTroops() - 1 ||
             numAttackingDice > 3 || cin.fail());
    return numAttackingDice;
}

static int getNumDefendingDice(Map::Country* toCountry) {

    int numDefendingDice = 0;
    do {
        std::cin.clear();
        std::cout << "[DEFENDER] How many dice will Player " << toCountry->getPlayerOwnerID() << " roll?(1 to ";
        if (toCountry->getNumberOfTroops() >= 2) {
            std::cout << "2)";
        } else {
            std::cout << "1)";
        }
        std::cin >> numDefendingDice;
        if (numDefendingDice < 1 || numDefendingDice > 2 || numDefendingDice > toCountry->getNumberOfTroops() - 1 ||
            cin.fail()) {
            std::cout << "\nInvalid Input. Please try again.\n";
            continue;
        }
    } while (numDefendingDice < 1 || numDefendingDice > 2 || numDefendingDice > toCountry->getNumberOfTroops() - 1 ||
             cin.fail());
    return numDefendingDice;
}

/**
 * The act of moving armies between adjacent and owned countries
 *
 * @param fromCountry the country to take armies from
 * @param countryToFortify  the country the player wants to fortify
 * @param numArmies  the number of armies to move
 */
int Player::executeFortify(Map::Country& fromCountry, Map::Country& countryToFortify, int numArmies) {
    /*
     * Act of moving armies between this player's owned countries.
     * fromCountry and countryToFortify have to be owned by this player and adjacent to each other.
     */

    fromCountry.setNumberOfTroops(fromCountry.getNumberOfTroops() - numArmies);
    countryToFortify.setNumberOfTroops(countryToFortify.getNumberOfTroops() + numArmies);
    std::cout << "\nPlayer " << this->getPlayerId() << " has fortified " << countryToFortify.getCountryName() << " from "
              << fromCountry.getCountryName() << std::endl;
    return PlayerAction::SUCCEEDED;
}

int Player::fortify() {

    char playerChoice = 0;
    int fromCountryIndex = -1;
    int ctryToFortIndex = -1;
    int numArmies = -1;

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
            showCountries(*this->getOwnedCountries());
            std::cout << "\nYou own " << this->getOwnedCountries()->size()
                      << " countries. Which country do you want to move armies from ? (choose 0 to "
                      << this->getOwnedCountries()->size() - 1 << " ) ";
            std::cin >> fromCountryIndex;
            //new condition
            if (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries()->size() - 1 || this->getOwnedCountries()->at(fromCountryIndex)->getNumberOfTroops() <= 1) {
                std::cout << "\nInvalid input. Try again.\n";
                continue;
            }
            //new condition
        } while (fromCountryIndex < 0 || fromCountryIndex > (int) this->getOwnedCountries()->size() - 1 || this->getOwnedCountries()->at(fromCountryIndex)->getNumberOfTroops() <= 1);
    } else {
        return PlayerAction::ABORTED;
    }

    Map::Country* fromCountry = this->getOwnedCountries()->at(fromCountryIndex);
    do {
        std::cout << "\nYou have " << fromCountry->getNumberOfTroops() << " armies in this country."
                  << " How many would you like to move ? (1 to " << fromCountry->getNumberOfTroops() - 1 << ") ";
        std::cin >> numArmies;
        if (numArmies < 1 || numArmies > fromCountry->getNumberOfTroops() - 1) {
            std::cout << "\nInvalid input. Try again.\n";
            continue;
        }
    } while (numArmies < 1 || numArmies > fromCountry->getNumberOfTroops() - 1);

    do {
        std::cout << "\nCountry " << fromCountry->getCountryName() << " has " << fromCountry->getAdjCountries()->size()
                  << " neighbours\n";\
        std::cout << "Which of YOUR countries would you like to move your armies to ? (0 to "
                  << fromCountry->getAdjCountries()->size() - 1 << ") ";
        std::cin >> ctryToFortIndex;

        if (ctryToFortIndex < 0 || ctryToFortIndex > (int) fromCountry->getAdjCountries()->size() - 1) {
            std::cout << "\nInvalid input. Try again.\n";
            continue;
        }
        if (getPlayerId() != fromCountry->getAdjCountries()->at(ctryToFortIndex)->getPlayerOwnerID() || fromCountry->getAdjCountries()->empty()) {
            std::cout << "\nEither you don't own this country or the country has no neighbours.\n"
                      << "Skip turn.\n" << std::endl;
            return -1;
        }
    } while (ctryToFortIndex < 0 || ctryToFortIndex > (int) fromCountry->getAdjCountries()->size() - 1 || getPlayerId() != fromCountry->getAdjCountries()->at(ctryToFortIndex)->getPlayerOwnerID());

    Map::Country* countryToFortify = fromCountry->getAdjCountries()->at(ctryToFortIndex);

    return this->executeFortify(*fromCountry, *countryToFortify, numArmies);
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

    // Roll
    std::vector<int> attackingRolls = this->getDiceRoller()->roll(numAttackingDice);
    std::vector<int> defendingRolls = defendingPlayer->getDiceRoller()->roll(numDefendingDice);

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
                        if (newArmies > fromCountry->getNumberOfTroops() - 1 || newArmies < 1 || cin.fail()) {
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
            std::cout << "\nAttacker lost 1 army!" << std::endl;
            fromCountry->setNumberOfTroops(fromCountry->getNumberOfTroops() - 1);
        }
    }
    return PlayerAction::SUCCEEDED;
}

// TODO: fix the reinforce method, see GameEngineDriver
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
    auto cardExchange = [](Player& player) {
        auto output = 0;

        while (true) {
            if (player.getCards()->getHand()->size() > 5) {
                std::cout << "You have more than 5 cards in your hand, so you must exchange at least once" << std::endl;
            } else if (canExchange(*player.getCards()->getHand())) {
                char input = 0;
                std::cout << "Would you like to exchange cards? (Y/n)";
                std::cin >> input;
                if (input == 'n' || input == 'N') {
                    return output;
                }
            } else {
                std::cout << "You don't have valid cards to exchange! Moving on..." << std::endl;
                return output;
            }
            std::cout << "What cards would you like to exchange?" << std::endl;
            auto types = std::vector<int>(3);
            types.reserve(3);
            types[0] = 0, types[1] = 0, types[2] = 0;
            for (auto card : *player.getCards()->getHand()) {
                switch (card) {
                    case CardType::INFANTRY: types[0]++; break;
                    case CardType::ARTILLERY: types[1]++; break;
                    case CardType::CAVALRY: types[2]++; break;
                    default: {
                        return output;
                    }
                }
            }

            std::cout << "You hand is: " << std::endl;
            std::cout << types[0] << " infantry, ";
            std::cout << types[1] << " artillery, and ";
            std::cout << types[2] << " cavalry" << std::endl;

            auto cardsToExchange = std::vector<CardType>();
            auto remaining = 3;

            while (remaining > 0) {
                std::cout << "You must pick " << remaining << " more cards to exchange" << std::endl;
                for (auto i = 0 ; i <= 2 ; i++) {
                    if (types[i] > 0) {
                        auto input = 0;
                        do {
                            std::cout << "How many " << (i == 0 ? "infantry" : i == 1 ? "artillery" : "cavalry");
                            std::cout << " would you like to exchange?";
                            std::cin >> input;
                            if (input > remaining) {
                                std::cout << "You can only exchange " << remaining << " more cards" << std::endl;
                            }
                            else if (input > types[i]) {
                                std::cout << "You cannot exchange more cards of a given type than you have in your hand.";
                            }
                            else {
                                for (auto j = 0 ; j < input ; j++, remaining--) {
                                    cardsToExchange.push_back((CardType) i);
                                }

                            }
                        } while ((input > types[i] || input > remaining) && remaining != 0);
                    }
                }
            }
            auto out = Hand::exchange(player.getCards(), GameLoop::getInstance()->getGameDeck(), cardsToExchange);
            if (out == -1) {
                std::cout << "An error occurred while exchanging your cards." << std::endl;
                return 0;
            }
            else {
                output += out;
            }
        }

        return output;
    };

    auto countriesOwned = [](const Player& player) {
        auto countries = player.getOwnedCountries()->size();

        return countries < 3 ? 3 : (int) countries / 3;
    };

    auto continentControlValue = [](const Player& player) {
        auto value = 0;
        for (auto* cont : *GameLoop::getInstance()->getGameMap()->getMapContinents()) {
            auto fullControl = true;

            for (auto* country : *cont->getCountriesInContinent()) {
                if (player.getPlayerId() != country->getPlayerOwnerID()) {
                    fullControl = false;
                    break;
                }
            }

            if (fullControl) {
                value += cont->getpCTroops();
            }
        }

        return value;
    };

    auto exchange = cardExchange(*this);
    if (exchange < 0) {
        return PlayerAction::FAILED;
    }

    auto newArmies = exchange +
            countriesOwned(*this) +
            continentControlValue(*this);

    std::cout << "Place your armies:" << std::endl;

    int troops, place = 0;
    while (newArmies > 0) {
        for (auto* country : *this->getOwnedCountries()) {
            std::cout << "Troops remaining: " << newArmies << std::endl;
            std::cout << country->getCountryName() << " has " << country->getNumberOfTroops()
                      << " armies. Add how many? ";
            // TODO: make sure the user enters an amount <= newArmies
            std::cin >> place;
            std::cout << std::endl;

            troops = country->getNumberOfTroops();
            country->setNumberOfTroops(troops + place);
            newArmies -= place;
            if (newArmies <= 0) {
                std::cout << "\nYou've placed all your armies!" << std::endl;
                break;
            }
        }
    }

    return PlayerAction::SUCCEEDED;
}

/**
 * Encapsulates the whole business code of the Player's attack behavior
 *
 * @return integer representing if the attack succeeded or not
 */
int Player::attack() {

    char playerChoice = 0;
    int fromCountryIndex;
    int toCountryIndex;
    int numAttackingDice;
    int numDefendingDice;

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

        if (playerChoice == 'y') {
            /*USER CHOOSES FROM WHICH COUNTRY TO ATTACK*/
            fromCountryIndex = getAttackingCountry(this);
        } else {
            return PlayerAction::ABORTED;
        }

        // Get the country to attack from
        Map::Country* fromCountry = this->getOwnedCountries()->at(fromCountryIndex);
        if (fromCountry->getNumberOfTroops() < 2) {
            std::cout << "Country " << fromCountry->getCountryName()
                      << " does not have enough armies to attack. Choose a different country. Proceeding...\n";
            continue;
        }

        /* USER CHOOSES WHICH COUNTRY TO ATTACK*/
        toCountryIndex = getDefendingCountry(fromCountry);

        Map::Country* toCountry = fromCountry->getAdjCountries()->at(toCountryIndex);
        if (fromCountry->getPlayerOwnerID() == toCountry->getPlayerOwnerID()) {
            std::cout << "\n[ATTACKER] You cannot attack your own country! Proceeding...\n\n";
            continue;
        }

        /*ATTACKER CHOOSES HOW MANY DICE TO ROLL*/
        numAttackingDice = getNumAttackingDice(this, fromCountry);

        /*DEFENDER CHOOSES HOW MANY DICE TO ROLL*/
        numDefendingDice = getNumDefendingDice(toCountry);

        /* GET THE DEFENDING PLAYER */
        Player* defendingPlayer = nullptr;
        for (auto& i : *GameLoop::getInstance()->getAllPlayers()) {
            if (i->getPlayerId() == toCountry->getPlayerOwnerID()) {
                defendingPlayer = i;
                break;
            }
        }
        if (defendingPlayer == nullptr) {
            return PlayerAction::FAILED;
        }

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

