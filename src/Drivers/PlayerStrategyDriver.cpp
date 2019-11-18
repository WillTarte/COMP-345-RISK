#include "../../include/GameEngine.h"

int main() {

    GameLoop::start();
    GameLoop::getInstance()->distributeArmies();
    GameLoop::getInstance()->loop();

    GameLoop::resetInstance();
    return 0;
}
