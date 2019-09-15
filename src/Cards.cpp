//
// Created by claudia on 9/12/2019.
//

#include <random>
#include <vector>
#include <algorithm>
#include "../include/Cards.h"

/**
 * Deck Constructor
 * @param amountCountries
 */
Deck::Deck(int amountCountries) {
    deckSize = new int(amountCountries); //store value on the heap to avoid losing it
    deckPointer = new std::vector<CardType>;
}

/**
 * Initial empty hand
 */
Hand::Hand() {
    handPointer = new std::vector<CardType>;
}

/**
 * Populates a deck of size equal to the number of countries
 * @return
 */
void Deck::createDeck() {
    std::vector<CardType> deck = Deck::populateDeck();
    std::shuffle (deck.begin(), deck.end(), std::mt19937(std::random_device()()));
    deckPointer = new std::vector<CardType>(deck); //store value on the heap to avoid losing it
}

/**
 * Add as many cards as there are counties to the game deck. Equal parts infantry, cavalry, and artillery
 * @return
 */
std::vector<CardType> Deck::populateDeck() {

    std::vector<CardType> deck = {};

    for (int i = 0; i < Deck::getNumberOfCards() / 3; i++) {
        deck.emplace_back(CardType::INFANTRY);
    }

    for (int i = 0; i < Deck::getNumberOfCards() / 3; i++) {
        deck.emplace_back(CardType::ARTILLERY);
    }

    for (int i = 0; i < Deck::getNumberOfCards() / 3; i++) {
        deck.emplace_back(CardType::CAVALRY);
    }

    Deck::setNumberOfCards(deckPointer->size());

    return deck;
}

/**
 * User draws a random card from the deck and it is removed from the deck
 */
void Deck::draw(Hand& userHand) {
    CardType drawnCard = deckPointer->back(); //save a copy of the last value of the vector
    std::cout << static_cast<int>(drawnCard) << std::endl;
    deckPointer->pop_back(); //remove the last value of the vector
    Deck::setNumberOfCards(deckPointer->size());
    userHand.getHand()->push_back(drawnCard);
}