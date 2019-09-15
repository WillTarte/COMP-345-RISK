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

class Hand {
public:
    Hand();
    std::vector<CardType>* getHand(){ return handPointer; }
    std::vector<CardType> *handPointer;

private:
};

class Deck {
public:
    Deck(int numberCountries);
    int getNumberOfCards(){ return *deckSize; }
    void setNumberOfCards(int newSize){ deckSize = new int(newSize); }
    std::vector<CardType>* getDeck(){ return deckPointer; }
    void createDeck();
    std::vector<CardType> populateDeck();
    void draw(Hand& userHand);

private:
    int *deckSize;
    std::vector<CardType> *deckPointer;
};

#endif //COMP_345_PROJ_CARDS_H
