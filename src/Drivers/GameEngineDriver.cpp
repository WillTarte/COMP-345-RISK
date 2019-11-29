//
// Created by claul on 10/27/2019.
//

#include "../../include/Map.h"
#include "../../include/Player.h"
#include "../../include/GameEngine.h"
#include <iostream>

using std::cout;
using std::vector;

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Main method ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

int main() {

    std::cout << "\033[34m";
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "------------ Running the game start phase --------------" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "\033[30m";

    cout << "\n";

    GameLoop::startSingle();
    
    return 0;
}