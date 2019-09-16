//
// Created by claudia on 9/12/2019.
//

#include <iostream>
#include "../../include/Cards.h"

int main() {

    int infantryCount = 0;
    int artilleryCount = 0;
    int cavalryCount = 0;

    Deck deck = Deck(9);
    deck.createDeck();

    int size = deck.getNumberOfCards();

    std::cout << "\n--------------------\n Creation of a Deck \n--------------------" << std::endl;

    for (int i = 0; i < size; i++) {

        if (deck.getDeck()->at(i) == 0) {
            std::cout << "Card: [INFANTRY]" << std::endl;
            infantryCount++;
        }if (deck.getDeck()->at(i) == 1) {
            std::cout << "Card: [ARTILLERY]" << std::endl;
            artilleryCount++;
        }if (deck.getDeck()->at(i) == 2) {
            std::cout << "Card: [CAVALRY]" << std::endl;
            cavalryCount++;
        }
    }

    std::cout << "\n\n------------------\n Deck Composition \n------------------" << std::endl;

    printf ("Number of [INFANTRY] cards: [%d]", infantryCount);
    printf ("\nNumber of [ARTILLERY] cards: [%d]", infantryCount);
    printf ("\nNumber of [CAVALRY] cards: [%d]", infantryCount);

    std::cout << "\n\n---------------\n Hand of Cards \n---------------" << std::endl;

    Hand hand = Hand();
    deck.draw(hand);
    deck.draw(hand);
    deck.draw(hand);

    int handSize = hand.getHand()->size();

    for (int i = 0; i < handSize; i++)  {
        if (hand.getHand()->at(i) == 0) {
            std::cout << "Card: [INFANTRY]" << std::endl;
            infantryCount++;
        }if (hand.getHand()->at(i) == 1) {
            std::cout << "Card: [ARTILLERY]" << std::endl;
            artilleryCount++;
        }if (hand.getHand()->at(i) == 2) {
            std::cout << "Card: [CAVALRY]" << std::endl;
            cavalryCount++;
        }
    }

    std::cout << "\n\n-------------------\n Exchange Function \n-------------------" << std::endl;

    std::vector<CardType> cardsExchanged = std::vector<CardType>();
    cardsExchanged.push_back(hand.getHand()->at(0));
    cardsExchanged.push_back(hand.getHand()->at(1));
    cardsExchanged.push_back(hand.getHand()->at(2));

    int armiesReceived = Hand::exchange(hand, deck, cardsExchanged);

    if (armiesReceived == -1) {
        std::cout << ("Invalid set of cards exchanged. They must either: All be of the same type or all be different.") << std::endl;
        std::cout <<  ("\nCards attempted to exchange:") << std::endl;

        for (int i = 0; i < 3; i++) {
            if (hand.getHand()->at(i) == 0) {
                std::cout << "Card: [INFANTRY]" << std::endl;
                infantryCount++;
            }if (hand.getHand()->at(i) == 1) {
                std::cout << "Card: [ARTILLERY]" << std::endl;
                artilleryCount++;
            }if (hand.getHand()->at(i) == 2) {
                std::cout << "Card: [CAVALRY]" << std::endl;
                cavalryCount++;
            }
        }

    }
    else {
        printf ("Number of armies received: %d", armiesReceived);
    }

    std::cout << "\n\n-----------------------\n Current Hand of Cards \n-----------------------" << std::endl;

    handSize = hand.getHand()->size();

    for (int i = 0; i < handSize; i++)  {
        if (hand.getHand()->at(i) == 0) {
            std::cout << "Card: [INFANTRY]" << std::endl;
            infantryCount++;
        }if (hand.getHand()->at(i) == 1) {
            std::cout << "Card: [ARTILLERY]" << std::endl;
            artilleryCount++;
        }if (hand.getHand()->at(i) == 2) {
            std::cout << "Card: [CAVALRY]" << std::endl;
            cavalryCount++;
        }
    }
}