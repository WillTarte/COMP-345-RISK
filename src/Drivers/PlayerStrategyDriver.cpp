#include "../../include/GameEngine.h"



int main() {

    char choice = '\0';
    GameLoop::start();
    GameLoop::getInstance()->distributeArmies();


    std::cout << "All players have been initialized. Do you want to make all players Humans?(y/n)";
    std::cin >> choice;
    if(choice == 'y') {
        for(auto* p : *GameLoop::getInstance()->getAllPlayers()) {
            std::cout << "Player " << p->getPlayerId() << " was " << p->getStrategyName() << std::endl;
            p->setPlayerStrategy(Strategies::HUMAN_PLAYER);
        }
    } else {
        for(auto* p : *GameLoop::getInstance()->getAllPlayers()) {
            std::cout << "Player " << p->getPlayerId() << " is " << p->getStrategyName() << std::endl;
        }
    }

    GameLoop::getInstance()->loop();

    GameLoop::resetInstance();
    return 0;
}