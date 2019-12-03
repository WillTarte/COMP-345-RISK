//
// Created by Willi on 11/10/2019.
//

#include <algorithm>
#include <iomanip>
#include "../include/Player.h"
#include "../include/GameObservers.h"
#include "../include/GameEngine.h"

PhaseObserver::PhaseObserver(Player* subject) {
    this->subject = subject;
    subject->attachObserver(this);
}

PhaseObserver::~PhaseObserver() {
    this->subject->detachObserver(this);
}

PhaseObserver::PhaseObserver(const PhaseObserver& toCopy) {
    this->subject = toCopy.subject;
    this->subject->attachObserver(this);
}

void PhaseObserver::operator=(const PhaseObserver& rhs) {
    this->subject = rhs.subject;
    this->subject->attachObserver(this);
}

void PhaseObserver::update() {

    if (this->subject->getOwnedCountries()->size() ==
        GameLoop::getInstance()->getGameMap()->getMapCountries()->size()) {
        return;
    } else {
        std::cout << std::endl << "#####################" << std::endl;
        std::cout << "Player " << this->subject->getPlayerId() << " is " << subject->getPlayerState() << std::endl;
        std::cout << "#####################" << std::endl << std::endl;

        switch (subject->getPlayerState()) {
            case ATTACKING:
                std::cout << "You own:" << std::endl;
                for (const auto& country : *subject->getOwnedCountries()) {
                    std::cout << '\t' << country->getCountryName() << ": " << country->getNumberOfTroops() << " armies"
                              << std::endl;
                    for (const auto& neighbour : *country->getAdjCountries()) {
                        if (neighbour->getPlayerOwnerID() != subject->getPlayerId()) {
                            std::cout << "\t\t-> " << neighbour->getCountryName() << ": owned by Player "
                                      << neighbour->getPlayerOwnerID() << ", with " << neighbour->getNumberOfTroops()
                                      << " armies" << std::endl;
                        }
                    }
                }
                break;
            case DEFENDING:
                break;
            case FORTIFYING:
                std::cout << "You own:" << std::endl;
                for (const auto& country : *subject->getOwnedCountries()) {
                    std::cout << '\t' << country->getCountryName() << ": " << country->getNumberOfTroops() << " armies"
                              << std::endl;
                    for (const auto& neighbour : *country->getAdjCountries()) {
                        if (neighbour->getPlayerOwnerID() == subject->getPlayerId()) {
                            std::cout << "\t\t-> " << neighbour->getCountryName() << ": "
                                      << neighbour->getNumberOfTroops()
                                      << " armies" << std::endl;
                        }
                    }
                }
                break;
            case REINFORCING:
                std::cout << "Your current hand: " << std::endl << '\t';
                std::cout
                        << std::count(subject->getCards()->getHand()->begin(), subject->getCards()->getHand()->end(), 0)
                        << " infantry, "
                        << std::count(subject->getCards()->getHand()->begin(), subject->getCards()->getHand()->end(), 1)
                        << " artillery, "
                        << std::count(subject->getCards()->getHand()->begin(), subject->getCards()->getHand()->end(), 2)
                        << " cavalry" << std::endl;
                std::cout << "You own: " << std::endl;
                for (const auto& country : *subject->getOwnedCountries()) {
                    std::cout << '\t' << country->getCountryName() << ": " << country->getNumberOfTroops() << " armies"
                              << std::endl;
                }
                break;
            case IDLE:
                break;
        }
    }
}

Player PhaseObserver::getSubject() const {
    return *this->subject;
}

PlayerState PhaseObserver::getSubjectState() const {
    return this->subject->getPlayerState();
}


StatsObserver::StatsObserver(Player* subject) {
    this->subject = subject;
    subject->attachObserver(this);
}

StatsObserver::~StatsObserver() {
    this->subject->detachObserver(this);
}

StatsObserver::StatsObserver(const StatsObserver& toCopy) {
    this->subject = toCopy.subject;
    this->subject->attachObserver(this);
}

void StatsObserver::operator=(const StatsObserver& rhs) {
    this->subject = rhs.subject;
    this->subject->attachObserver(this);
}

void StatsObserver::update() {
    bool gameDone = false;
    int playerId = 0;

    if (this->subject->getOwnedCountries()->size() ==
        GameLoop::getInstance()->getGameMap()->getMapCountries()->size()) {
        gameDone = true;
        playerId = this->subject->getPlayerId();
    }
    if (gameDone) {
        cout << "\033[1m\033[33m";
        std::cout << "Player " << playerId << " owns all of the countries! They have won the game!!";
    } else {
        StatsObserver::printTable();
    }
}

/**
 * Center the text in the column
 * @param str
 * @param width
 * @return
 */
std::string center(const string& str, const int width) {
    stringstream ss, spaces;
    int padding = width - str.size();                 // count excess room to pad
    for (int i = 0; i < padding / 2; ++i)
        spaces << " ";
    ss << spaces.str() << str << spaces.str();    // format with padding
    if (padding > 0 && padding % 2 != 0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}

/**
 * Print each row
 */
void StatsObserver::printStats() {
    int numberOfCountries = GameLoop::getInstance()->getGameMap()->getMapCountries()->size();
    std::stringstream stream;

    for (auto* p : *GameLoop::getInstance()->getAllPlayers()) {
        if (!p->getOwnedCountries()->empty()) {
            std::string currentPlayer = "Player " + std::to_string(p->getPlayerId());
            std::string percentage = std::to_string(
                    static_cast<double>(p->getOwnedCountries()->size()) / static_cast<double>(numberOfCountries) * 100) + "%";
            std::cout << center(currentPlayer, 10) << " | ";
            std::cout << center(std::to_string(p->getOwnedCountries()->size()), 20) << " | ";
            std::cout << center(percentage, 20) << "\n";
        }
    }
}

/**
 * Print headers + rows
 */
void StatsObserver::printTable() {
    std::cout << "\n\n--------------------------------------------------------\n";
    std::cout << "--------------------Game Statistics---------------------\n";
    std::cout << "--------------------------------------------------------\n\n";

    std::cout << center("Player", 10) << " | "
              << center("Countries Owned", 20) << " | "
              << center("% of the Map Owned", 20) << "\n";

    for (int i = 0; i < 56; i++) {
        std::cout << "-";
    }
    std::cout << "\n";

    StatsObserver::printStats();
}



