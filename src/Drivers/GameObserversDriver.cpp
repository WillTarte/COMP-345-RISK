//
// Created by Admin on 11/12/2019.
//


#include <GameEngine.h>
#include <GameObservers.h>

int main() {

    GameLoop::start();
    Player* p1 = GameLoop::getInstance()->getAllPlayers()->at(0);
    p1->notifyAll();
    p1->setPlayerState(PlayerState::ATTACKING);
    p1->notifyAll();
    GameLoop::resetInstance();
    return 0;
}
