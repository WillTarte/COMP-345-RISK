//
// Created by claudia on 9/12/2019.
//

#include <random>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "../include/Cards.h"
using namespace std;

namespace globalHandVariables {
    int tradedSetsCount = 1;
}

/**
 * Deck Constructor
 * @param amountCountries
 */
Deck::Deck(int amountCountries) {
    deckSize = new int(amountCountries); //store value on the heap to avoid losing it
    deckPointer = new std::vector<CardType>;
    discardPointer = new std::vector<CardType>;
}

/**
 * Deck destructor
 */
Deck::~Deck() {
    delete deckSize;
    delete deckPointer;
    delete discardPointer;
}

/**
 * Deck copy constructor
 * @param toCopy
 */
Deck::Deck(const Deck &toCopy) {
    deckSize = new int();
    deckPointer = new vector<CardType>;
    discardPointer = new vector<CardType>;
    *deckSize = *toCopy.deckSize;
    *deckPointer = *toCopy.deckPointer;
    *discardPointer = *toCopy.discardPointer;
}

/**
 * assignment operator
 */
void Deck::operator=(Deck& rhs) {
    this->deckPointer = rhs.deckPointer;
    this->deckSize = rhs.deckSize;
    this->discardPointer = rhs.discardPointer;
}

/**
 * Hand Constructor
 */
Hand::Hand() {
    handPointer = new std::vector<CardType>;
}

/**
 * Hand Destructor
 */
Hand::~Hand() {
    delete handPointer;
}

/**
 * Hand copy constructor
 * @param toCopy
 */
Hand::Hand(const Hand &toCopy) {
    handPointer = new std::vector<CardType>;
    *handPointer = *toCopy.handPointer;
}

/**
 * assignment operator
 * @param rhs
 */
void Hand::operator=(Hand& rhs) {
    this->handPointer = rhs.handPointer;
}

/**
 * Populates a deck of size equal to the number of countries
 * @return
 */
void Deck::createDeck() {
    std::vector<CardType> deck = Deck::populateDeck();
    std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device()()));
    deckPointer = new std::vector<CardType>(deck); //store value on the heap to avoid losing it
}

/**
 * Add as many cards as there are counties to the game deck. Equal parts infantry, cavalry, and artillery
 * @return
 */
std::vector<CardType> Deck::populateDeck() {
    std::vector<CardType> deck = {};

    for (int i = 0; i < *Deck::getNumberOfCards() / 3; i++) {
        deck.emplace_back(CardType::INFANTRY);
        deck.emplace_back(CardType::ARTILLERY);
        deck.emplace_back(CardType::CAVALRY);
    }

    Deck::setNumberOfCards(deck.size());

    return deck;
}

/**
 * User draws a random card from the deck and it is removed from the deck
 */
void Deck::draw(Hand& userHand) {
    CardType drawnCard = deckPointer->back();
    deckPointer->pop_back();
    Deck::setNumberOfCards(deckPointer->size());
    userHand.getHand()->push_back(drawnCard);
}

/**
 * exchange three cards of kind of three different cards to gain army members
 * @param deck
 * @return
 */
int Hand::exchange(Hand* hand, Deck* deck, std::vector<CardType>* givenCards) {
    if (givenCards->size() != 3) {
        return -1;
    }
        //Ensure that all three given cards are the same
    else if (givenCards->at(0) == givenCards->at(1) && givenCards->at(1) == givenCards->at(2)) {
        int armiesToExchange = Hand::armiesReceived();

        try {
            deck->discard(hand, givenCards);
        } catch (const std::length_error& handLength) {
            std::cout << handLength.what() << std::endl;
            exit(1);
        }

        globalHandVariables::tradedSetsCount++;
        return armiesToExchange;
    }
        //Ensure that all three cards are different
    else if (givenCards->at(0) != givenCards->at(1) &&
             givenCards->at(0) != givenCards->at(2) &&
             givenCards->at(1) != givenCards->at(2)) {
        int armiesToExchange = Hand::armiesReceived();

        try {
            deck->discard(hand, givenCards);
        } catch (const std::length_error& handLength) {
            std::cout << handLength.what() << std::endl;
            exit(1);
        }

        globalHandVariables::tradedSetsCount++;
        return armiesToExchange;
    }
    return -1;
}

/**
 * Determine the number of armies received through card exchange
 * @return
 */
int Hand::armiesReceived() {
    if (globalHandVariables::tradedSetsCount < 6) {
        return globalHandVariables::tradedSetsCount * 2 + 2;
    } else if (globalHandVariables::tradedSetsCount == 6) {
        return 15;
    }
    return 15 + (globalHandVariables::tradedSetsCount - 6) * 5;
}

/**
 * Puts exchanged cards in the discard pile & removes them from hand
 * @param discardedCards
 */
void Deck::discard(Hand* hand, std::vector<CardType>* discardedCards) {

    unsigned int initialHandSize = hand->getHand()->size();

    for (int i = 0; i < 3; i++) {
        Deck::discardPointer->push_back(discardedCards->at(i));
    }

    for (int i = 0; i < 3; i++) {
        unsigned int counter = 0; // position in vector
        int removedCard = 0;

        while (removedCard == 0 && counter <
                                   hand->getHand()->size()) { //stop once the vector has been gone through completely or a card was removed
            if (hand->getHand()->at(counter) == discardedCards->at(i)) {
                hand->getHand()->erase(hand->getHand()->begin() + counter);
                removedCard = 1;
            } else {
                counter++;
            }
        }
    }

    // checks if the cards were removed from the hand as expected
    if (hand->getHand()->size() == initialHandSize) {
        throw std::length_error(
                "The three cards were not removed from the hand once exchanged successfully. System exiting.");
    }
}
