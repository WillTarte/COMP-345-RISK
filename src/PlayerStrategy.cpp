#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <GameEngine.h>
#include "../include/Player.h"
#include "../include/PlayerStrategy.h"
#include "../include/Map.h"

/******************************************************************************/
/*********************** Player strategy constructors *************************/
/******************************************************************************/
PlayerStrategy::PlayerStrategy() {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("ABSTRACT");
}

PlayerStrategy::PlayerStrategy(Player* player) {
    this->exchangingCardType = new int(0);
    this->armiesToPlace = new int(0);
    this->player = player;
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string(player->getStrategy()->getStrategyName());
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
    this->strategyName = new std::string(toCopy.getStrategyName());
}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
    this->strategyName = new std::string(rhs.getStrategyName());
}

void PlayerStrategy::resetChoices() {
    this->from = nullptr;
    this->to = nullptr;
    this->armiesToPlace = nullptr;
    this->exchangingCardType = nullptr;
    this->numWeakest = new int(-1);
}

PlayerStrategy::~PlayerStrategy() {
    delete from;
    delete to;
    delete armiesToPlace;
    delete exchangingCardType;
    delete numWeakest;
    delete strategyName;
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
    this->strategyName = new std::string("HUMAN");
}

HumanPlayerStrategy::HumanPlayerStrategy(const Player& player) {
    this->exchangingCardType = new int(0);
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("HUMAN");
}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& toCopy)  : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
    this->strategyName = new std::string("HUMAN");
}

HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
    this->strategyName = new std::string("HUMAN");
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
    this->strategyName = new std::string("AGGRESSIVE");
}

AggressiveBotStrategy::AggressiveBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->exchangingCardType = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("AGGRESSIVE");
}

AggressiveBotStrategy::AggressiveBotStrategy(const AggressiveBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
    this->strategyName = new std::string("AGGRESSIVE");
}

AggressiveBotStrategy& AggressiveBotStrategy::operator=(const AggressiveBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
    this->strategyName = new std::string("AGGRESSIVE");
}

/******************************************************************************/
/*********************** Benevolent bot constructors **************************/
/******************************************************************************/
BenevolentBotStrategy::BenevolentBotStrategy() {
    this->armiesToPlace = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("BENEVOLENT");
}

BenevolentBotStrategy::BenevolentBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("BENEVOLENT");
}

BenevolentBotStrategy::BenevolentBotStrategy(const BenevolentBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
    this->strategyName = new std::string("BENEVOLENT");
}

BenevolentBotStrategy& BenevolentBotStrategy::operator=(const BenevolentBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
    this->strategyName = new std::string("BENEVOLENT");
}

/******************************************************************************/
/************************* Random bot constructors ****************************/
/******************************************************************************/

RandomBotStrategy::RandomBotStrategy() {
    this->armiesToPlace = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("RANDOM");
}

RandomBotStrategy::RandomBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("RANDOM");
}
RandomBotStrategy::RandomBotStrategy(const RandomBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
    this->strategyName = new std::string("RANDOM");
}

RandomBotStrategy& RandomBotStrategy::operator=(const RandomBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
    this->strategyName = new std::string("RANDOM");
}

/******************************************************************************/
/************************* Cheater bot constructors ****************************/
/******************************************************************************/

CheaterBotStrategy::CheaterBotStrategy(){
    this->armiesToPlace = new int(0);
    this->player = nullptr;
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("CHEATER");
}

CheaterBotStrategy::CheaterBotStrategy(const Player& player) {
    this->armiesToPlace = new int(0);
    this->player = new Player(player);
    this->from = nullptr;
    this->to = nullptr;
    this->strategyName = new std::string("CHEATER");
}
CheaterBotStrategy::CheaterBotStrategy(const CheaterBotStrategy& toCopy) : PlayerStrategy(toCopy) {
    this->exchangingCardType = toCopy.exchangingCardType;
    this->armiesToPlace = toCopy.armiesToPlace;
    this->player = toCopy.player;
    this->from = toCopy.from;
    this->to = toCopy.to;
    this->strategyName = new std::string("CHEATER");
}

CheaterBotStrategy& CheaterBotStrategy::operator=(const CheaterBotStrategy& rhs) {
    this->exchangingCardType = rhs.exchangingCardType;
    this->armiesToPlace = rhs.armiesToPlace;
    this->player = rhs.player;
    this->from = rhs.from;
    this->to = rhs.to;
    this->strategyName = new std::string("CHEATER");
}

/******************************************************************************/
/******************** Human player strategy methods ***************************/
/******************************************************************************/
signed char HumanPlayerStrategy::yesOrNo(StrategyContext _) {
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
signed char AggressiveBotStrategy::yesOrNo(StrategyContext context) {
    char botChoice = 0;
    switch ((int) context) {
        case StrategyContext::ATTACK:
            botChoice = canAttack() ? 'y' : 'n';
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

bool AggressiveBotStrategy::canAttack() {

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
            count = reinforceArmyCount();
            break;
        case StrategyContext::REINFORCE_CARD_COUNT:
            count = numCards();
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
    unsigned int fromCountryIndex = -1;
    Map::Country* biggestCountry = nullptr;
    for (auto* country : *player->getOwnedCountries()) {
        if (biggestCountry == nullptr) {
            biggestCountry = country;
        } else if (biggestCountry->getNumberOfTroops() < country->getNumberOfTroops()) {
            for (auto* neighbour : *country->getAdjCountries()) {
                if (neighbour->getPlayerOwnerID() != country->getPlayerOwnerID()) {
                    biggestCountry = country;
                }
            }
        }
    }

    from = biggestCountry;
    for (unsigned long i = 0; i < player->getOwnedCountries()->size(); i++) {
        if (player->getOwnedCountries()->at(i)->getCountryId() == biggestCountry->getCountryId()) {
            fromCountryIndex = i;
        }
    }

    return (int) fromCountryIndex;
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
    unsigned int index = 0;
    for (unsigned long i = 0; i < from->getAdjCountries()->size(); i++) {
        if (smallestNeighbour == nullptr && from->getAdjCountries()->at(i)->getPlayerOwnerID() != from->getPlayerOwnerID()) {
            smallestNeighbour = from->getAdjCountries()->at(i);
            index = i;
        }
        if (smallestNeighbour != nullptr && smallestNeighbour->getNumberOfTroops() > from->getAdjCountries()->at(i)->getNumberOfTroops() && from->getAdjCountries()->at(i)->getPlayerOwnerID()!= from->getPlayerOwnerID()) {
            smallestNeighbour = from->getAdjCountries()->at(i);
            index = i;
        }
    }

    to = smallestNeighbour;
    return (int) index;
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
    int fromCountryIndex = 0;
    int numTroops = 9999;
    for(int i = 0; i < player->getOwnedCountries()->size(); i++){
        if(player->getOwnedCountries()->at(i)->getNumberOfTroops() < numTroops && player->getOwnedCountries()->at(i)->getNumberOfTroops() > 1){
            fromCountryIndex = i;
            numTroops = player->getOwnedCountries()->at(i)->getNumberOfTroops();
            from = player->getOwnedCountries()->at(i);
        }
    }
//    from = nullptr;
//    std::set<int> checkedBiggestCountries = std::set<int>();
//    unsigned int fromCountryIndex = -1;
//    while (from == nullptr) {
//        Map::Country* biggestCountry = nullptr;
//        for (auto* country : *player->getOwnedCountries()) {
//            if (biggestCountry != nullptr && checkedBiggestCountries.find(biggestCountry->getCountryId()) != checkedBiggestCountries.end()) {
//                continue;
//            }
//            if (biggestCountry == nullptr || biggestCountry->getNumberOfTroops() < country->getNumberOfTroops()) {
//                biggestCountry = country;
//            }
//        }
//
//        for (auto* neighbour : *biggestCountry->getAdjCountries()) {
//            if (neighbour->getPlayerOwnerID() == biggestCountry->getPlayerOwnerID() && neighbour->getNumberOfTroops() > 1 ) {
//                from = neighbour;
//            }
//        }
//
//        if (from == nullptr) {
//            checkedBiggestCountries.insert(biggestCountry->getCountryId());
//            continue;
//        }
//
//        fromCountryIndex = -1;
//        for (unsigned long i = 0; i < player->getOwnedCountries()->size(); i++) {
//            if (player->getOwnedCountries()->at(i)->getCountryId() == from->getCountryId()) {
//                fromCountryIndex = i;
//            }
//        }
//    }

    return fromCountryIndex;
}

/**
 * This function is just going to find the biggest country the player owns
 * and use that country as the one to fortify.
 *
 * The expected output is the same as the attackFromCountry function
 **/
int AggressiveBotStrategy::fortifyToCountryIndex() {
    int toCountry = 0;
    int numTroops = 2;
    for(int i = 0; i < from->getAdjCountries()->size(); i++){
        if(from->getAdjCountries()->at(i)->getNumberOfTroops() >= numTroops){
            toCountry = i;
            numTroops = from->getAdjCountries()->at(i)->getNumberOfTroops();
            to = from->getAdjCountries()->at(i);
        }
    }

    return toCountry;
//    to = nullptr;
//    for (auto* country : *player->getOwnedCountries()) {
//        if (to == nullptr) {
//            to = country;
//        }
//        else if (country->getNumberOfTroops() > to->getNumberOfTroops()) {
//            to = country;
//        }
//    }
//
//    unsigned int toCountryRelativeIndex = -1;
//    for (unsigned long i = 0; i < from->getAdjCountries()->size(); i++) {
//        if (from->getAdjCountries()->at(i)->getCountryId() == to->getCountryId()) {
//            toCountryRelativeIndex = i;
//        }
//    }
//
//    return (int) toCountryRelativeIndex;
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
int AggressiveBotStrategy::numCards() {
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
int AggressiveBotStrategy::reinforceArmyCount() {
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

    bool canFortify = false;
    Map::Country* biggestCountry = nullptr;
    for (auto* country: *player->getOwnedCountries()) {
        if (biggestCountry == nullptr || biggestCountry->getNumberOfTroops() < country->getNumberOfTroops()) {
            biggestCountry = country;
        }
    }
    for (auto* neighbour : *biggestCountry->getAdjCountries()) {
        if (neighbour->getPlayerOwnerID() == biggestCountry->getPlayerOwnerID() && neighbour->getNumberOfTroops() > 1) {
            canFortify = true;
        }
    }

    return canFortify;
}

/******************************************************************************/
/******************* Benevolent bot strategy methods **************************/
/******************************************************************************/
signed char BenevolentBotStrategy::yesOrNo(StrategyContext context) {
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
            count = place();
            break;
        case StrategyContext::REINFORCE_CARD_COUNT:
            count = numArmies();
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
 * The expected output of this function is the index of the biggest country that
 * the benevolent bot player controls.
 **/
int BenevolentBotStrategy::fortifyFromCountryIndex() {
    unsigned int fromCountryIndex = 0;
    int numTroops = -1;
    for(int i = 0; i < player->getOwnedCountries()->size(); i++){
        if(player->getOwnedCountries()->at(i)->getNumberOfTroops() > numTroops){
            fromCountryIndex = i;
            numTroops = player->getOwnedCountries()->at(i)->getNumberOfTroops();
            from = player->getOwnedCountries()->at(i);
        }
    }
    //    from = nullptr;
//    std::set<int> checkedSmallestCountries = std::set<int>();
//    Map::Country* smallestCountry = nullptr;
//    for(auto* country : *player->getOwnedCountries()) {
//        if(smallestCountry == nullptr) {
//            smallestCountry = country;
//            from = country;
//        } else if (smallestCountry->getNumberOfTroops() > country->getNumberOfTroops()) {
//            for(auto* neighbour : *country->getAdjCountries()) {
//                if(neighbour->getPlayerOwnerID() == country->getPlayerOwnerID() && neighbour->getNumberOfTroops() > country->getNumberOfTroops()) {
//                    smallestCountry = country;
//                    from = neighbour;
//                }
//            }
//        }
//    }
//    for (int i = 0; i < player->getOwnedCountries()->size();i++) {
//        if(player->getOwnedCountries()->at(i)->getCountryId() == from->getCountryId()) {
//            fromCountryIndex = i;
//        }
//    }
    return (int) fromCountryIndex;
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

//    to = nullptr;
//    Map::Country* smallestNeighbour = nullptr;
//    for(auto* neighbour : *from->getAdjCountries()) {
//        if(smallestNeighbour == nullptr && neighbour->getPlayerOwnerID() == from->getPlayerOwnerID()) {
//            smallestNeighbour = neighbour;
//            continue;
//        }
//        if(neighbour < smallestNeighbour && neighbour->getPlayerOwnerID() == from->getPlayerOwnerID()) {
//            smallestNeighbour = neighbour;
//        }
//    }
//
//    to = smallestNeighbour;
//    unsigned int toCountryRelativeIndex = -1;
//    for (unsigned long i = 0 ; i < from->getAdjCountries()->size() ; i++) {
//        if (from->getAdjCountries()->at(i)->getCountryId() == to->getCountryId()) {
//            toCountryRelativeIndex = i;
//        }
//    }
    int toCountry = 0;
    int numTroops = 9999;

    for(int i = 0; i < from->getAdjCountries()->size(); i++){
        if(from->getAdjCountries()->at(i)->getNumberOfTroops() < numTroops){
            toCountry = i;
            numTroops = from->getAdjCountries()->at(i)->getNumberOfTroops();
            to = from->getAdjCountries()->at(i);
        }
    }

    return toCountry;
}

/**
 * This will split the number of armies between the two countries evenly so that
 * both countries end up with the same number of armies.
 **/
int BenevolentBotStrategy::fortifyArmyCount() {
    int move = from->getNumberOfTroops();
    move = (move - to->getNumberOfTroops()) / 2;
    //bug fix : so that it doesnt try to move a negative number of armies
    if(move < 0){
        return 0;
    }
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
        if (smallestCountry == nullptr) {
            smallestCountry = country;
        } else if (smallestCountry->getNumberOfTroops() > country->getNumberOfTroops()) {
            for (auto* neighbour : *country->getAdjCountries()) {
                if (neighbour->getPlayerOwnerID() == country->getPlayerOwnerID()
                    && neighbour->getNumberOfTroops() > country->getNumberOfTroops()) {
                    smallestCountry = country;
                    canFortify = true;
                }
            }
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

/******************************************************************************/
/********************* Random bot strategy methods ****************************/
/******************************************************************************/

/**
 * Randomly decides if the bot will do an action
 * @param context
 * @return
 */
signed char RandomBotStrategy::yesOrNo(StrategyContext context) {
    std::vector<char> optionVector = {'y', 'n'};
    std::shuffle(optionVector.begin(), optionVector.end(), std::mt19937(std::random_device()())); // randomize the option vector
    switch((int) context) {
        case StrategyContext::ATTACK:
            std::cout << ((optionVector[0] == 'y') ? " yes, master" : " no, master") << std::endl;
            break;
        case StrategyContext::FORTIFY:
            std::cout << ((optionVector[0] == 'y') ? " yes, master" : " no, master") << std::endl;
            break;
        case StrategyContext::REINFORCE:
            std::cout << ((optionVector[0] == 'y') ? " yes, master" : " no, master") << std::endl;
            break;
    }
    return optionVector[0];
}

/**
 * Determine the maximal number of dice the random bot can throw during an attack (including 0)
 * @return
 */
int RandomBotStrategy::attackMaxDice(std::mt19937 gen) {
    int maxDice = 1;
    if (from->getNumberOfTroops() > 3) {
        maxDice = 3;
    }
    else if (from->getNumberOfTroops() == 3) {
        maxDice = 2;
    }
    std::uniform_int_distribution<> dis(1, maxDice);
    return dis(gen);
}

/**
 * Select a random country that the user owns
 * @return
 */
int RandomBotStrategy::chooseRandomCountry(std::mt19937 gen) {
    int numberOfCountries = this->player->getOwnedCountries()->size();
    std::uniform_int_distribution<> dis(0, numberOfCountries - 1); // define the range
    return dis(gen);
}

int RandomBotStrategy::chooseRandomNeighbour(std::mt19937 gen) {
    int numberOfNeighbours = this->from->getAdjCountries()->size();
    std::uniform_int_distribution<> dis(0, numberOfNeighbours - 1);
    return dis(gen);
}

int RandomBotStrategy::sendRandomArmies(std::mt19937 gen) {
    int maxArmies = this->from->getNumberOfTroops() - 1;
    std::uniform_int_distribution<> dis(0, maxArmies);
    return dis(gen);
}

int RandomBotStrategy::reinforceMaxCards(std::mt19937 gen, int currentType) {
    int numberCurrentType = 0;
    for (CardType currentCard : *player->getCards()->getHand()) {
        if (currentCard == currentType) {
            numberCurrentType++;
        }
    }
    std::uniform_int_distribution<> dis(0, numberCurrentType);
    return dis(gen);
}

/**
 * Provide a random player input for all prompts in all phases
 * @param context
 * @return
 */
int RandomBotStrategy::intInput(StrategyContext context) {
    int userInput = 0;
    std::random_device rd; // random number generator
    std::mt19937 gen(rd()); // seed the generator
    switch((int) context) {
        case StrategyContext::ATTACK_FROM_COUNTRY: {
            userInput = RandomBotStrategy::chooseRandomCountry(gen);
            from = player->getOwnedCountries()->at(userInput);
        } break;
        case StrategyContext::ATTACK_TO_COUNTRY: {
            userInput = RandomBotStrategy::chooseRandomNeighbour(gen);
            to = from->getAdjCountries()->at(userInput);
        } break;
        case StrategyContext::ATTACK_DICE_COUNT: {
            userInput = RandomBotStrategy::attackMaxDice(gen);
        } break;
        case StrategyContext::DEFEND_DICE_COUNT: {
            int maxDice = (to->getNumberOfTroops() >= 2) ? 2 : 1;
            std::uniform_int_distribution<> dis(1, maxDice);
            userInput = dis(gen);
        } break;
        case StrategyContext::FORTIFY_FROM_COUNTRY: {
            userInput = RandomBotStrategy::chooseRandomCountry(gen);
            from = player->getOwnedCountries()->at(userInput);
        } break;
        case StrategyContext::FORTIFY_TO_COUNTRY: {
            userInput = RandomBotStrategy::chooseRandomNeighbour(gen);
            to = from->getAdjCountries()->at(userInput);
        } break;
        case StrategyContext::FORTIFY_ARMY_COUNT: {
            userInput = RandomBotStrategy::sendRandomArmies(gen);
        } break;
        case StrategyContext::ATTACK_NEW_ARMY_COUNT: {
            userInput = RandomBotStrategy::sendRandomArmies(gen);
        } break;
        case StrategyContext::REINFORCE_CARD_COUNT: {
            userInput = RandomBotStrategy::reinforceMaxCards(gen, *exchangingCardType);
        } break;
        case StrategyContext::REINFORCE_ARMY_COUNT: {
            std::uniform_int_distribution<> dis(0, *armiesToPlace);
            userInput = dis(gen);
        }
    }
    std::cout << " " << userInput << std::endl;
    return userInput;
}

/******************************************************************************/
/********************* Cheater bot strategy methods ****************************/
/******************************************************************************/

signed char CheaterBotStrategy::yesOrNo(StrategyContext context) {

    switch (context) {
        case ATTACK:
            return (signed char) this->cheaterAttack();
        case REINFORCE:
            return (signed char) this->cheaterReinforce();
        case FORTIFY:
            return (signed char) this->cheaterFortify();
        default:
            return -1;
    }
}

int CheaterBotStrategy::intInput(StrategyContext context) {
    return 0;
}

int CheaterBotStrategy::cheaterReinforce() {
    std::cout << "We have a cheater! They are doubling all armies on their countries" << std::endl;
    for(auto* country : *this->player->getOwnedCountries()) {
        std::cout << "[CHEATER] - Country " << country->getCountryName() << " had " << country->getNumberOfTroops() << " armies. It now has ";
        //prevent overflow
        if(country->getNumberOfTroops() * 2 > 0 && country->getNumberOfTroops() * 2 < 2147483647){
            country->setNumberOfTroops(country->getNumberOfTroops() * 2);
        }
        std::cout << country->getNumberOfTroops() << " armies. -" << std::endl;
    }

    player->setPlayerState(PlayerState::IDLE);
    player->getStrategy()->resetChoices();
    return PlayerAction::SUCCEEDED;
}

bool CheaterBotStrategy::exchangeCountryOwnership(Player* defendingPlayer, Map::Country* toExchange) {
    int toExchangeIndex = -1;
    for (int i = 0; i < defendingPlayer->getOwnedCountries()->size(); i++) {
        if (defendingPlayer->getOwnedCountries()->at(i)->getCountryId() == toExchange->getCountryId()) {
            if (toExchangeIndex != -1) {
                return false;
            }
            toExchangeIndex = i;
        }
    }
    if (toExchangeIndex == -1) {
        return false;
    } else {
        this->player->getOwnedCountries()->push_back(defendingPlayer->getOwnedCountries()->at(toExchangeIndex));
        defendingPlayer->getOwnedCountries()->erase(defendingPlayer->getOwnedCountries()->begin() + toExchangeIndex);
        return true;
    }
}

int CheaterBotStrategy::cheaterAttack() {
    std::cout << "We have a cheater! They are conquering all their neighbouring countries" << std::endl;
    for (auto* country : *this->player->getOwnedCountries()) {
        for (auto* neighbour : *country->getAdjCountries()) {
            if (neighbour->getPlayerOwnerID() == this->player->getPlayerId()) {
                continue;
            }
            Player* defendingPlayer = nullptr;
            for (auto* i : *GameLoop::getInstance()->getAllPlayers()) {
                if (i->getPlayerId() == neighbour->getPlayerOwnerID()) {
                    defendingPlayer = i;
                }
            }
            if (defendingPlayer == nullptr) {
                player->setPlayerState(PlayerState::IDLE);
                player->getStrategy()->resetChoices();
                return PlayerAction::FAILED;
            }
            if (exchangeCountryOwnership(defendingPlayer, neighbour)) {
                std::cout << "[CHEATER] - Country " << neighbour->getCountryName() << " has been attacked & conquered by Player " << this->player->getPlayerId() << " -\n";
                continue;
            } else {
                player->setPlayerState(PlayerState::IDLE);
                player->getStrategy()->resetChoices();
                return PlayerAction::FAILED;
            }
        }
    }
    player->setPlayerState(PlayerState::IDLE);
    player->getStrategy()->resetChoices();
    return PlayerAction::SUCCEEDED;
}

int CheaterBotStrategy::cheaterFortify() {
    std::cout << "We have a cheater! They are fortifying all their countries with enemy neighbours" << std::endl;
    for (auto* country : *this->player->getOwnedCountries()) {
        bool canFortify = false;
        for (auto* neighbour : *country->getAdjCountries()) {
            if (neighbour->getPlayerOwnerID() != country->getPlayerOwnerID()) {
                canFortify = true;
            }
        }
        if (canFortify) {
            std::cout << "[CHEATER] - Country " << country->getCountryName() << " had " << country->getNumberOfTroops() << " armies. It now has ";
            //prevent overflow
            if(country->getNumberOfTroops() * 2 > 0 && country->getNumberOfTroops() * 2 < 2147483647){
                country->setNumberOfTroops(country->getNumberOfTroops() * 2);
            }
            std::cout << country->getNumberOfTroops() << " armies. -" << std::endl;
        }
    }

    player->setPlayerState(PlayerState::IDLE);
    player->getStrategy()->resetChoices();
    return PlayerAction::SUCCEEDED;
}
