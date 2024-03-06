//
// Created by yagiz on 2/22/2024.
//


#ifndef BLACKJACK_REAL_DECK_H
#define BLACKJACK_REAL_DECK_H

#include "vector"
#include "fstream"

struct Deck
{
    std::vector<int> cards;
    unsigned int getNumberOfCards();
    void removeCard(unsigned int cardToAdd);
    void addCard(unsigned int cardToAdd);
    void clearDeck();
    void printCards();
    void equalizeDeck(const Deck& deckToCopy);
    unsigned int getElement(unsigned int index);
    void writeCards(std::fstream &txt);
    std::vector<std::vector<unsigned int>> getCardCombinations(unsigned int selectionSize);
    void generateCombinations(unsigned int selectionSize, unsigned int startIndex, std::vector<unsigned int>& currentCombination, std::vector<std::vector<unsigned int>>& combinations);

};

struct PlayerDeck:public Deck
{
    unsigned int getNumberOfAces();
    unsigned int getOpenCardValue();
    unsigned int getTotalValue();
    unsigned int getGameValue();

};

class GameDeck:public Deck
{
public:
    unsigned int deckMultiplier;
    void createDebugDeck();
    void createLargeDeck();
    void resetDeck();
    void removeCardsForTreeOperation(PlayerDeck nodeCards);
    unsigned int getRandomCard();
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

#endif //BLACKJACK_REAL_DECK_H
