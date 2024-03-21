//
// Created by yagiz on 2/22/2024.
//


#ifndef BLACKJACK_REAL_DECK_H
#define BLACKJACK_REAL_DECK_H

#include "vector"
#include "fstream"
#include "iostream"
#include <random>


typedef unsigned int card_t;

struct Deck
{
    std::vector<card_t> cards;
    unsigned int getNumberOfCards();
    void removeCard(card_t cardToRemove);
    void addCard(card_t cardToAdd);
    void clearDeck();
    void printCards();
    void copyDeck(const Deck& deckToCopy);
    card_t getElement(unsigned int index);
    void writeCards(std::fstream &txt);
    std::vector<std::vector<card_t>> getCardCombinations(unsigned int selectionSize);
    void generateCombinations(unsigned int selectionSize, unsigned int startIndex, std::vector<unsigned int>& currentCombination, std::vector<std::vector<unsigned int>>& combinations);

};

struct PlayerDeck:public Deck
{
    unsigned int getNumberOfAces();
    card_t getOpenCard();
    unsigned int getTotalValue();
    unsigned int getGameValue();

};

class GameDeck:public Deck
{
public:
    std::mt19937 rng;
    unsigned int deckMultiplier;
    void createDebugDeck();
    void createLargeDeck();
    void resetDeck();
    void removeCardsForTreeOperation(PlayerDeck nodeCards);
    card_t getRandomCard();
    GameDeck(unsigned int deckMultiplier);
    std::vector<PlayerDeck> createCombinationHands(std::vector<std::vector<unsigned int>> combinationVector);
};

struct CardRemovalError : public std::exception
{
    const char* what();
};

struct OutOfCards : public std::exception
{
    const char* what();
};

struct AsyncDecks : public std::exception
{
    const char* what();
};

struct NotThreadApplicable : public std::exception
{
    const char* what();
};

#endif //BLACKJACK_REAL_DECK_H
