//
// Created by yagiz on 2/22/2024.
//


#ifndef BLACKJACK_REAL_DECK_H
#define BLACKJACK_REAL_DECK_H

#include "vector"

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
};
#endif //BLACKJACK_REAL_DECK_H
