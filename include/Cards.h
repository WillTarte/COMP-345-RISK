//
// Created by claudia on 9/12/2019.
//

#include <iostream>
#include <vector>
#include <string>
#ifndef COMP_345_PROJ_CARDS_H
#define COMP_345_PROJ_CARDS_H

enum class CardType {
    INFANTRY = 0,
    ARTILLERY = 1,
    CAVALRY = 2
};

class Deck {
public:
    Deck(int numberCountries);
    int getNumberOfCards(){ return *deckSize; }
    std::vector<CardType>* getDeck(){ return deckPointer; }
    void createDeck();
    std::vector<CardType> populateDeck();

private:
    int *deckSize;
    std::vector<CardType> *deckPointer;
};

#endif //COMP_345_PROJ_CARDS_H
