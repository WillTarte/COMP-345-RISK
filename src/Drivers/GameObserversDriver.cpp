//
// Created by Admin on 11/12/2019.
//


#include <GameEngine.h>

int main() {

    GameLoop::start();

    Player* p1 = GameLoop::getInstance()->getAllPlayers()->at(0);
    // TODO: why the fuck is this giving a compilation error
    PhaseObserver po = PhaseObserver(p1);
}
