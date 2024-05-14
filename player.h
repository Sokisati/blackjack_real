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
    unsigned int getPlayerGameValue();
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
    double getExpectedValue(GameDeck originalDeck, card_t openCard);
    unsigned int getImaginaryHandValue(card_t imaginaryCardToDraw);
    void drawSpecificCard(card_t cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck);
    void drawRandomCard(GameDeck &actualDeck, GameDeck &knownDeck);
    double expectedValueCaseDetector(double expectedValue,double initialWinProb,const unsigned int &iteration,const unsigned int &deckSize);
    unsigned int getImaginaryHandValueCombinationHand(PlayerDeck playerDeckToAdd);
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
