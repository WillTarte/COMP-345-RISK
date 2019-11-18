#include <iostream>
#include <math.h>
#include "../include/Player.h"
#include "../include/PlayerStrategy.h"
#include "../include/Map.h"

// TODO: fix output for bot players

/******************************************************************************/
/*********************** Player strategy constructors *************************/
/******************************************************************************/
PlayerStrategy::PlayerStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

PlayerStrategy::PlayerStrategy(Player* player) {
    this->exchangingCardType = new int(0);
    this->armiesToPlace = new int(0);
    this->player = player;
    this->from = nullptr;
    this->to = nullptr;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

void PlayerStrategy::resetChoices() {
    this->from = nullptr;
    this->to = nullptr;
    this->armiesToPlace = nullptr;
    this->exchangingCardType = nullptr;
    this->numWeakest = nullptr;
}

PlayerStrategy::~PlayerStrategy() {
    delete from;
    delete to;
    delete armiesToPlace;
    delete exchangingCardType;
    delete numWeakest;
    // makes sure the player is not deleted when switching strategies
    player = nullptr;
    delete player;
}

/******************************************************************************/
/*********************** Player strategy constructors *************************/
/******************************************************************************/
HumanPlayerStrategy::HumanPlayerStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

HumanPlayerStrategy::HumanPlayerStrategy(const Player& player) {
    this->exchangingCardType = new int(0);
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/*********************** Aggressive bot constructors **************************/
/******************************************************************************/
AggressiveBotStrategy::AggressiveBotStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

AggressiveBotStrategy::AggressiveBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

AggressiveBotStrategy::AggressiveBotStrategy(const AggressiveBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

AggressiveBotStrategy& AggressiveBotStrategy::operator=(const AggressiveBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/*********************** Benevolent bot constructors **************************/
/******************************************************************************/
BenevolentBotStrategy::BenevolentBotStrategy() {
    this->armiesToPlace = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
}

BenevolentBotStrategy::BenevolentBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
}

BenevolentBotStrategy::BenevolentBotStrategy(const BenevolentBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
}

BenevolentBotStrategy& BenevolentBotStrategy::operator=(const BenevolentBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
}

/******************************************************************************/
/******************** Human player strategy methods ***************************/
/******************************************************************************/
char HumanPlayerStrategy::yesOrNo(StrategyContext _) {
    /*
     * Need the parameter to generalize the function, but human inputs
     * don't need context. If you don't do anything with the parameter,
     * it throws a warning. So this way we get to generalize the solution
     * by just throwing away the parameter immediately.
     */
    char playerDecision = 0;
    std::cin >> playerDecision;
    std::cin.ignore(512, '\n');
    return playerDecision;
}

int HumanPlayerStrategy::intInput(StrategyContext _) {
    int count = 0;
    std::cin >> count;
    std::cin.ignore(512, '\n');
    return count;
}

/******************************************************************************/
/******************* Aggressive bot strategy methods **************************/
/******************************************************************************/
char AggressiveBotStrategy::yesOrNo(StrategyContext context) {
    char botChoice = 0;
    switch ((int) context) {
        case StrategyContext::ATTACK:
            botChoice = willAttack() ? 'y' : 'n';
            break;
        case StrategyContext::FORTIFY:
            botChoice = canFortify() ? 'y' : 'n';
            break;
        case StrategyContext::REINFORCE:
            botChoice = 'y';
            break;
        default: {
            // Should never occur
            botChoice = 'n';
        }
    }
    std::cout << botChoice << std::endl;
    return botChoice;
}

bool AggressiveBotStrategy::willAttack() {

    Map::Country* biggest = nullptr;
    bool canAttack = false;
    std::set<int> checkedCountries = std::set<int>();

    while(!canAttack) {
        for (auto* country : *player->getOwnedCountries()) {
            if (biggest == nullptr) {
                biggest = country;
            } else if (biggest->getNumberOfTroops() < country->getNumberOfTroops() ||
                       checkedCountries.find(biggest->getCountryId()) != checkedCountries.end()) {
                for (auto* neighbour : *country->getAdjCountries()) {
                    if (neighbour->getPlayerOwnerID() != country->getPlayerOwnerID()) {
                        biggest = country;
                    }
                }
            }
        }

        for (auto* neighbour: *biggest->getAdjCountries()) {
            if (neighbour->getPlayerOwnerID() != biggest->getPlayerOwnerID()) {
                canAttack = true;
            }
        }
        if (!canAttack) {
            checkedCountries.insert(biggest->getCountryId());
        }
    }

    if (from != nullptr && biggest->getCountryId() != from->getCountryId()) {
        canAttack = false;
    }

    return canAttack;
}

/**
 * This function will delegate which function to use depending on what you're trying to do.
 **/
int AggressiveBotStrategy::intInput(StrategyContext context) {
    int count = 0;

    switch ((int) context) {
        case StrategyContext::ATTACK_FROM_COUNTRY:
            count = attackFromCountryIndex();
            break;
        case StrategyContext::ATTACK_TO_COUNTRY:
            count = attackToCountryIndex();
            break;
        case StrategyContext::ATTACK_DICE_COUNT:
            count = attackNumDice();
            break;
        case StrategyContext::ATTACK_NEW_ARMY_COUNT:
            count = 1;
            break;
        case StrategyContext::DEFEND_DICE_COUNT:
            count = defendNumDice();
            break;
        case StrategyContext::FORTIFY_FROM_COUNTRY:
            count = fortifyFromCountryIndex();
            break;
        case StrategyContext::FORTIFY_TO_COUNTRY:
            count = fortifyToCountryIndex();
            break;
        case StrategyContext::FORTIFY_ARMY_COUNT:
            count = fortifyArmyCount();
            break;
        case StrategyContext::REINFORCE_ARMY_COUNT:
            count = place();
            break;
        case StrategyContext::REINFORCE_CARD_COUNT:
            count = numArmies();
            break;
        default: {
            count = -1;
        }
    }
    std::cout << count << std::endl;
    return count;
}

/**
 * This function will loop over all the countries you own and find the one that has the
 * most number of troops.
 *
 * The expected output is the index of the biggest (attack valid) country a player owns.
 **/
int AggressiveBotStrategy::attackFromCountryIndex() {

    int fromCountryIndex = -1;
    Map::Country* biggestCountry = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if(biggestCountry == nullptr) {
            biggestCountry = country;
        } else if(biggestCountry->getNumberOfTroops() < country->getNumberOfTroops()) {
            for(auto* neighbour : *country->getAdjCountries()) {
                if(neighbour->getPlayerOwnerID() != country->getPlayerOwnerID()) {
                    biggestCountry = country;
                }
            }
        }
    }

    from = biggestCountry;
    for(int i = 0; i < player->getOwnedCountries()->size(); i++) {
        if(player->getOwnedCountries()->at(i)->getCountryId() == biggestCountry->getCountryId()) {
            fromCountryIndex = i;
        }
    }

    return fromCountryIndex;
}

/**
 * This method will look at all countries that are adjacent to the attacking
 * player's biggest country. It will find it's weakest country and attack that.
 *
 * If the attacking player controls all adjacent countries, then it'll reselect
 * which country to attack from - not sure if this should happen since this isn't
 * defined behaviour in the assingment?
 *
 * The expected output is the smallest enemry country adjacent to the attacker's biggest country.
 **/
int AggressiveBotStrategy::attackToCountryIndex() {
    Map::Country* smallestNeighbour = nullptr;
    int index = 0;
    for(int i = 0; i < from->getAdjCountries()->size(); i++) {
        if(smallestNeighbour == nullptr && from->getAdjCountries()->at(i)->getPlayerOwnerID() != from->getPlayerOwnerID()) {
            smallestNeighbour = from->getAdjCountries()->at(i);
            index = i;
        }
        if(smallestNeighbour != nullptr && smallestNeighbour->getNumberOfTroops() > from->getAdjCountries()->at(i)->getNumberOfTroops() && from->getAdjCountries()->at(i)->getPlayerOwnerID()!= from->getPlayerOwnerID()) {
            smallestNeighbour = from->getAdjCountries()->at(i);
            index = i;
        }
    }

    to = smallestNeighbour;
    return index;
}

/**
 * Attack with the most number of dice they can. Uses the same logic to find the upper
 * limits of how many dice they can attack with, and attacks with as much as possible.
 **/
int AggressiveBotStrategy::attackNumDice() {
    int numDice = 0;
    if (from->getNumberOfTroops() > 3) {
        numDice = 3;
    }
    else {
        numDice = from->getNumberOfTroops() - 1;
    }

    return numDice;
}

/**
 * Defend with the maximum number of dice. Just always fight back as much as they can.
 **/
int AggressiveBotStrategy::defendNumDice() {
    int numDice = 0;
    if (to->getNumberOfTroops() >= 2) {
        numDice = 2;
    }
    else {
        numDice = 1;
    }

    return numDice;
}

/**
 * Finds the smallest country the aggressive bot owns, and fortifies from that country.
 *
 * The expected output is the smallest country they own.
 **/
int AggressiveBotStrategy::fortifyFromCountryIndex() {

    from = nullptr;
    std::set<int> checkedBiggestCountries = std::set<int>();
    int fromCountryIndex = -1;
    while (from == nullptr) {
        Map::Country* biggestCountry = nullptr;
        for(auto* country : *player->getOwnedCountries()) {
            if (biggestCountry != nullptr && checkedBiggestCountries.find(biggestCountry->getCountryId()) != checkedBiggestCountries.end()) {
                continue;
            }
            if(biggestCountry == nullptr || biggestCountry->getNumberOfTroops() < country->getNumberOfTroops()) {
                biggestCountry = country;
            }
        }

        for (auto* neighbour : *biggestCountry->getAdjCountries()) {
            if(neighbour->getPlayerOwnerID() == biggestCountry->getPlayerOwnerID() && neighbour->getNumberOfTroops() > 1 ) {
                from = neighbour;
            }
        }

        if(from == nullptr) {
            checkedBiggestCountries.insert(biggestCountry->getCountryId());
            continue;
        }

        fromCountryIndex = -1;
        for(int i = 0; i < player->getOwnedCountries()->size(); i++) {
            if(player->getOwnedCountries()->at(i)->getCountryId() == from->getCountryId()) {
                fromCountryIndex = i;
            }
        }
    }

    return fromCountryIndex;
}

/**
 * This function is just going to find the biggest country the player owns
 * and use that country as the one to fortify.
 *
 * The expected output is the same as the attackFromCountry function
 **/
int AggressiveBotStrategy::fortifyToCountryIndex() {
    to = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (to == nullptr) {
            to = country;
        }
        else if (country->getNumberOfTroops() > to->getNumberOfTroops()) {
            to = country;
        }
    }

    return to->getCountryId();
}

/**
 * Just send all your troops from weaker countries to your biggest one to give it
 * as much strength as possible.
 **/
int AggressiveBotStrategy::fortifyArmyCount() {
    return from->getNumberOfTroops() - 1;
}

/**
 * Finds out how many cards of a certain type the player owns. Without any strategy,
 * it'll simply exchange all cards of that type.
 **/
int AggressiveBotStrategy::numArmies() {
    int count = 0;
    for (auto cardType : *player->getCards()->getHand()) {
        if ((int) cardType == *exchangingCardType) {
            count++;
        }
    }

    return count;
}

/**
 * The reinforce method asks if you'd like to reinforce at each country you own.
 * What this method does is it checks if that country is the biggest one you own,
 * and if it is, it'll return all the armies you're allowed to place.
 *
 * If the country the reinforce is prompting for is not the biggest country,
 * then it'll just return 0 so as to not reinforce anything but the biggest country.
 **/
int AggressiveBotStrategy::place() {
    Map::Country* biggest = nullptr;
    // Find what the biggest country is
    for (auto* country : *player->getOwnedCountries()) {
        if (biggest == nullptr || country->getNumberOfTroops() > biggest->getNumberOfTroops()) {
            biggest = country;
        }
    }

    if (biggest != nullptr && biggest->getCountryId() == to->getCountryId()) {
        std::cout << *armiesToPlace;
        return *armiesToPlace;
    }
    else {
        std::cout << "0";
        return 0;
    }
}

/**
 * Checks if the bot can fortify (biggest country has an adjacent country with > 1 army)
 *
 * @return true if the bot can fortify
 */
bool AggressiveBotStrategy::canFortify() {

    bool fortifiable = false;
    Map::Country* biggestCountry = nullptr;
    for(auto* country: *player->getOwnedCountries()) {
        if(biggestCountry == nullptr || biggestCountry->getNumberOfTroops() < country->getNumberOfTroops()) {
            biggestCountry = country;
        }
    }
    for(auto* neighbour : *biggestCountry->getAdjCountries()) {
        if(neighbour->getPlayerOwnerID() == biggestCountry->getPlayerOwnerID() && neighbour->getNumberOfTroops() > 1) {
            fortifiable = true;
        }
    }

    return fortifiable;
}

/******************************************************************************/
/******************* Benevolent bot strategy methods **************************/
/******************************************************************************/
char BenevolentBotStrategy::yesOrNo(StrategyContext context) {
    char botChoice = 0;
    switch ((int) context) {
        case StrategyContext::ATTACK:
            botChoice = 'n';
            break;
        case StrategyContext::FORTIFY:
            botChoice = canFortify() ? 'y' : 'n';
            break;
        case StrategyContext::REINFORCE:
            botChoice = 'y';
            break;
        default: {
            // Should never occur
            botChoice = 'n';
        }
    }
    std::cout << botChoice << std::endl;
    return botChoice;
}

/**
 * This function will delegate which function to use depending on what you're trying to do.
 **/
int BenevolentBotStrategy::intInput(StrategyContext context) {
    int count = 0;

    switch ((int) context) {
        case StrategyContext::DEFEND_DICE_COUNT:
            count = defendNumDice();
            break;
        case StrategyContext::FORTIFY_FROM_COUNTRY:
            count = fortifyFromCountryIndex();
            break;
        case StrategyContext::FORTIFY_TO_COUNTRY:
            count = fortifyToCountryIndex();
            break;
        case StrategyContext::FORTIFY_ARMY_COUNT:
            count = fortifyArmyCount();
            break;
        case StrategyContext::REINFORCE_ARMY_COUNT:
            count = numArmies();
            break;
        case StrategyContext::REINFORCE_CARD_COUNT:
            count = place();
            break;
        case StrategyContext::ATTACK_FROM_COUNTRY:
        case StrategyContext::ATTACK_TO_COUNTRY:
        case StrategyContext::ATTACK_DICE_COUNT:
        case StrategyContext::ATTACK_NEW_ARMY_COUNT:
        default: {
            count = -1;
        }
    }
    std::cout << count << std::endl;
    return count;
}

/**
 * This is the exact same strategy that the aggressive player uses. It will simply
 * always select the most dice it's allowed to for defending.
 **/
int BenevolentBotStrategy::defendNumDice() {
    int numDice = 0;
    if (to->getNumberOfTroops() >= 2) {
        numDice = 2;
    }
    else {
        numDice = 1;
    }

    return numDice;
}

/**
 * This is the opposite of the aggressive player. The benevolent bot will find out
 * what the biggest country they own is and use that to fortify their smaller
 * countries.
 *
 * The expected output of this function is the ID of the biggest country that
 * the benevolent bot player controls.
 **/
int BenevolentBotStrategy::fortifyFromCountryIndex() {
    from = nullptr;
    std::set<int> checkedSmallestCountries = std::set<int>();
    int fromCountryIndex = -1;
    while (from == nullptr) {
        Map::Country* smallestCountry = nullptr;

        for (auto* country : *player->getOwnedCountries()) {
            if (smallestCountry != nullptr && checkedSmallestCountries.find(smallestCountry->getCountryId()) != checkedSmallestCountries.end()) {
                continue;
            }
            if (smallestCountry == nullptr || smallestCountry->getNumberOfTroops() > country->getNumberOfTroops()) {
                smallestCountry = country;
            }
        }

        for (auto* neighbour : *smallestCountry->getAdjCountries()) {
            if (neighbour->getCountryId() == smallestCountry->getPlayerOwnerID() && neighbour->getNumberOfTroops() > smallestCountry->getNumberOfTroops()) {
                from = neighbour;
            }
        }

        if (from == nullptr) {
            checkedSmallestCountries.insert(smallestCountry->getCountryId());
            continue;
        }

        fromCountryIndex = -1;
        for (unsigned long i = 0 ; i < player->getOwnedCountries()->size(); i++) {
            if (player->getOwnedCountries()->at(i)->getCountryId() == from->getCountryId()) {
                fromCountryIndex = i;
            }
        }
    }

    return fromCountryIndex;
}

/**
 * Again, this is the opposite of the aggressive player. The benevolent bot will
 * find out what the smallest countries they own are and use that to fortify
 * from their bigger countries.
 *
 * The expected output of this function is the ID of the smallest country that
 * the benevolent bot player controls.
 **/
int BenevolentBotStrategy::fortifyToCountryIndex() {
    to = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (to == nullptr) {
            to = country;
        }
        else if (country->getNumberOfTroops() < to->getNumberOfTroops()) {
            to = country;
        }
    }

    return to->getCountryId();
}

/**
 * This will split the number of armies between the two countries evenly so that
 * both countries end up with the same number of armies.
 **/
int BenevolentBotStrategy::fortifyArmyCount() {
    int move = from->getNumberOfTroops();
    move = (move - to->getNumberOfTroops()) / 2;

    return move;
}

/**
 * Same as the aggressive bot, the benevolent bot will chose cards
 * to exchange without really applying any strategy to the decision.
 **/
int BenevolentBotStrategy::numArmies() {
    int count = 0;
    for (auto cardType : *player->getCards()->getHand()) {
        if ((int) cardType == *exchangingCardType) {
            count++;
        }
    }

    return count;
}

/**
 * numWeakest is initialized to -1, so if it's not yet been set then it'll set it.
 * To set numWeakest it first finds out what the player's weakest country is. Once
 * you know the weakest country, it'll then loop over all of that player's countries
 * again and find out how many countries have the same number of troops as that
 * country.
 *
 * Finally, it distributes armies across all it's weakest countries evenly. Therefore,
 * it'll find out how many armies you need to place and divide it by the number
 * of countries that you want to place on.
 *
 * It'll return the ceiling of that number so that it's placing at minimum 1 per country
 * instead of performing integer division and trying to place 0.
 **/
int BenevolentBotStrategy::place() {
    if (*numWeakest == -1) {
        int count = 0;
        int weakest = 0;
        // Find what the smallest country is
        for (auto* country : *player->getOwnedCountries()) {
            if (weakest == 0 || country->getNumberOfTroops() < weakest) {
                weakest = country->getNumberOfTroops();
            }
        }

        // Then find out how many other countries have the same number of armies as it
        for (auto* country : *player->getOwnedCountries()) {
            if (country->getNumberOfTroops() == weakest) {
                count++;
            }
        }

        numWeakest = new int(count);
    }

    return ceil((double)*armiesToPlace / (double)*numWeakest);
}

/**
 * Checks if the bot can fortify (smallest country has an adjacent country with more armies)
 *
 * @return true if the bot can fortify
 */
bool BenevolentBotStrategy::canFortify() {
    bool canFortify = false;
    Map::Country* smallestCountry = nullptr;
    for (auto* country: *player->getOwnedCountries()) {
        if (smallestCountry == nullptr || smallestCountry->getNumberOfTroops() > country->getNumberOfTroops()) {
            smallestCountry = country;
        }
    }

    for (auto* neighbour : *smallestCountry->getAdjCountries()) {
        if (neighbour->getPlayerOwnerID() == smallestCountry->getPlayerOwnerID()
                && neighbour->getNumberOfTroops() > smallestCountry->getNumberOfTroops()) {
            canFortify = true;
        }
    }

    return canFortify;
}
