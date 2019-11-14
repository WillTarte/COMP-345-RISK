//
// Created by Willi on 11/10/2019.
//

#include <algorithm>
#include "../include/Player.h"
#include "../include/GameObservers.h"

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
    std::cout << std::endl <<"#####################" << std::endl;
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
                        std::cout << "\t\t-> " << neighbour->getCountryName() << ": " << neighbour->getNumberOfTroops()
                                  << " armies" << std::endl;
                    }
                }
            }
            break;
        case REINFORCING:
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
            break;
    }
}

Player PhaseObserver::getSubject() const {
    return *this->subject;
}

PlayerState PhaseObserver::getSubjectState() const {
    return this->subject->getPlayerState();
}


