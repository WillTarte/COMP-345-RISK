//
// Created by Willi on 11/10/2019.
//
#include <Player.h>
#include "../include/GameObservers.h"

PhaseObserver::PhaseObserver(Player* subject) {
    this->subject = subject;
}

void PhaseObserver::update() {
    std::cout << "#####################" << std::endl;
    std::cout << "#Player " << this->subject->getPlayerId() << " is " << subject->getPlayerState() << "#" << std::endl;
    std::cout << "#####################" << std::endl;
}
