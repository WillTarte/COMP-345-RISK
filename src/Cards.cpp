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
    deckSize = &amountCountries;
    deckPointer = {};
}

/**
 * Populates a deck of size equal to the number of countries
 * @return
 */
void Deck::createDeck() {
    std::vector<CardType> deck = Deck::populateDeck();
    std::shuffle (deck.begin(), deck.end(), std::mt19937(std::random_device()()));
    deckPointer = new std::vector<CardType>(deck);
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

    return deck;
}