#include <iostream>
#include "include/Cards.h"
int main() 
{
    Deck a = Deck(9);
    a.createDeck();
    int size = a.getNumberOfCards();
    std::vector<CardType> *deckPointer = a.getDeck();

    for (int i = 0; i < size; i++) {
        std::cout << static_cast<int>(deckPointer->at(i)) << std::endl;
    }

    return 0;
}
