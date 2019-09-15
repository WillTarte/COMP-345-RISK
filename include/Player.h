//
// Created by William Tarte on 9/15/2019.
//

#include <iostream>
#include <vector>

#ifndef COMP_345_PROJ_PLAYER_H
#define COMP_345_PROJ_PLAYER_H


class Player {

    public:

        Deck* getDeckPointer();
        std::vector<Country>* getCountriesPointer();
        Dice* getDice();

        void reinforce(Country* fromCountry, Country* toCountry);
        void attack(Country* fromCountry, Country* toCountry);
        void fortify(Country* country, int numberOfArmies);

        Player();

    private:
        Hand* handPointer;
        std::vector<Countries>* countriesPointer;
        Dice* dice;


};


#endif //COMP_345_PROJ_PLAYER_H
