//
// Created by yagiz on 2/22/2024.
//

#ifndef BLACKJACK_REAL_PLAYER_H
#define BLACKJACK_REAL_PLAYER_H
#include "deck.h"
#include "tree_and_bar.h"

class Player{
public:
    unsigned int roundScore = 0;
    unsigned int simulationScore = 0;
    PlayerDeck cardsInsideHand;
    void printSubjectCards();
    unsigned getPlayerOpenCardValue();
    unsigned getPlayerGameValue();
    std::vector<int> getCards();
    void clearHand();
    void drawSpecificCard(unsigned int cardToDraw,GameDeck &actualDeck);
    void addRoundScore();
    void drawImaginaryCard(unsigned int cardToDraw);
    void drawRandomCard(GameDeck &actualDeck);
};

class Glados:public Player
{
public:
    ProbBar treeFunction(GameDeck originalDeck, unsigned int openCardValue);
    double getExpectedValue(GameDeck originalDeck, unsigned int openCardValue);
    unsigned int getImaginaryHandValue(unsigned int imaginaryCardToDraw);
    void drawSpecificCard(unsigned int cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck);
    void drawRandomCard(GameDeck &actualDeck, GameDeck &knownDeck);
    unsigned int getLastCard();
    void drawCardBasedOnExpectedValue(GameDeck &actualDeck, GameDeck &knownDeck, const unsigned int &dealerOpenCardValue);
};

class Dealer: public Player
{
public:
    void drawCardSoft17(GameDeck &actualDeck, GameDeck &knownDeck);
};

class DealerCopycat : public Dealer
{
public:
    bool canMirrorCard(Glados glados);
    void mirrorCard(Glados glados);
    void drawCardLikeDealer(Glados glados, GameDeck ghostDeck);
};

#endif //BLACKJACK_REAL_PLAYER_H
