//
// Created by Willi on 11/10/2019.
//

#include <algorithm>
#include "../include/GameObservers.h"
//#include "../include/Player.h"

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

std::ostream& operator<<(std::ostream& os, const PlayerState state) {
    switch (state) {
        case ATTACKING:
            os << "Attacking";
            break;
        case DEFENDING:
            os << "Defending";
            break;
        case FORTIFYING:
            os << "Fortifying";
            break;
        case REINFORCING:
            os << "Reinforcing";
            break;
        case IDLE:
            os << "Idle";
            break;
    }
}

void PhaseObserver::update() {
    std::cout << "#####################" << std::endl;
    std::cout << "#Player " << this->subject->getPlayerId() << " is " << subject->getPlayerState() << "#" << std::endl;
    std::cout << "#####################" << std::endl << std::endl;

    switch (subject->getPlayerState()) {
        case ATTACKING:
            // Show owned countries and their best possible target
            std::cout << "You own:" << std::endl;
            for (const auto& country : *subject->getOwnedCountries()) {
                std::cout << '\t' << country->getCountryName() << ": " << country->getNumberOfTroops() << " armies"
                          << std::endl;
                for (const auto& neighbour : *country->getAdjCountries()) {
                    if (neighbour->getPlayerOwnerID() != subject->getPlayerId()) {
                        std::cout << "\t\t" << neighbour->getCountryName() << ": owned by Player "
                                  << neighbour->getPlayerOwnerID() << ", with " << neighbour->getNumberOfTroops()
                                  << " armies" << std::endl;
                    }
                }
            }
            break;
        case DEFENDING:
            // don't do anything
            break;
        case FORTIFYING:
            // Show owned countries, and show the neighbours with biggest army delta
            std::cout << "You own:" << std::endl;
            for (const auto& country : *subject->getOwnedCountries()) {
                std::cout << '\t' << country->getCountryName() << ": " << country->getNumberOfTroops() << " armies"
                          << std::endl;
                for (const auto& neighbour : *country->getAdjCountries()) {
                    if (neighbour->getPlayerOwnerID() == subject->getPlayerId()) {
                        std::cout << "\t\t" << neighbour->getCountryName() << ": " << neighbour->getNumberOfTroops()
                                  << " armies" << std::endl;
                    }
                }
            }
            break;
        case REINFORCING:
            // Show player's hand, and country with the lowest amount of armies
            std::cout << "Your current hand: " << std::endl << '\t';
            std::cout << std::count(subject->getCards()->getHand()->begin(), subject->getCards()->getHand()->end(), 0)
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
            // Don't do shit
            break;
    }
}


