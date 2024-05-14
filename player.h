//
// Created by yagiz on 2/22/2024.
//

#ifndef BLACKJACK_REAL_PLAYER_H
#define BLACKJACK_REAL_PLAYER_H
#include "deck.h"
#include "tree_and_bar.h"

typedef unsigned int handvalue_t;

class Player{
public:
    unsigned int roundScore = 0;
    unsigned int simulationScore = 0;
    PlayerDeck cardsInsideHand;
    void printSubjectCards();
    handvalue_t getPlayerGameValue();
    void clearHand();
    void drawSpecificCard(card_t cardToDraw,GameDeck &actualDeck);
    void addRoundScore();
    void drawImaginaryCard(card_t cardToDraw);
    void drawRandomCard(GameDeck &actualDeck);
};

class Glados:public Player
{
public:
    ProbBar treeFunction(GameDeck originalDeck,card_t openCardValue);
    double getExpectedValue(GameDeck originalDeck, card_t openCard);
    handvalue_t getImaginaryHandValue(card_t imaginaryCardToDraw);
    void drawSpecificCard(card_t cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck);
    void drawRandomCard(GameDeck &actualDeck, GameDeck &knownDeck);
    double expectedValueCaseDetector(double expectedValue,double initialWinProb,const unsigned int &iteration,const unsigned int &deckSize);
    handvalue_t getImaginaryHandValueCombinationHand(PlayerDeck playerDeckToAdd);
};

class Dealer: public Player
{
public:
    card_t getPlayerOpenCard();
    void drawCardSoft17(GameDeck &actualDeck);
};

class DealerCopycat : public Dealer
{
public:
    bool canMirrorCard(Glados glados);
    void mirrorCard(Glados glados);
    void drawCardLikeDealer(const Glados& glados, GameDeck ghostDeck);
};

#endif //BLACKJACK_REAL_PLAYER_H
