//
// Created by claudia on 9/12/2019.
//

#include <iostream>
#include <vector>
#include <string>

#ifndef COMP_345_PROJ_CARDS_H
#define COMP_345_PROJ_CARDS_H

enum CardType {
    INFANTRY, ARTILLERY, CAVALRY
};

class Deck;

class Hand {
public:
    Hand();
    ~Hand();
    Hand(Hand& toCopy);
    void operator=(Hand& rhs);
    inline std::vector<CardType>* getHand() { return handPointer; }

    static int exchange(Hand* hand, Deck* deck, std::vector<CardType> givenCards);
    static int armiesReceived();
private:
    std::vector<CardType>* handPointer;
};

class Deck {
public:
    Deck(int numberCountries);
    ~Deck();
    Deck(const Deck& toCopy);
    void operator=(Deck& rhs);
    int* getNumberOfCards() { return deckSize; }
    void setNumberOfCards(int newSize) { deckSize = new int(newSize); }
    inline std::vector<CardType>* getDeck() { return deckPointer; }
    inline std::vector<CardType>* getDiscard() { return discardPointer; }
    void createDeck();
    std::vector<CardType> populateDeck();
    void draw(Hand& userHand);

    void discard(Hand* hand, std::vector<CardType> discardedCards);

private:
    int* deckSize;
    std::vector<CardType>* deckPointer;
    std::vector<CardType>* discardPointer;
};

#endif //COMP_345_PROJ_CARDS_H
