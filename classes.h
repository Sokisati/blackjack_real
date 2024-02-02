//
// Created by yagiz on 1/29/2024.
//
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

struct Deck
{
    std::vector<int> cards;
    unsigned int numberOfCards;
public:
    unsigned int getNumberOfCards();
    void removeCard(unsigned int cardToAdd);
    void addCard(unsigned int cardToAdd);
    void clearDeck();
    void printCards();
    void equalizeDeck(Deck deckToCopy);
    unsigned int getElement(unsigned int index);


};

struct PlayerDeck:public Deck
{
    unsigned int totalValue;
    unsigned int gameValue;
    unsigned int openCardValue;
    unsigned int getNumberOfAces();
    unsigned int getOpenCardValue();
    void calculateTotalValue();
    unsigned int getGameValue();
};

struct TreeNode
{
    PlayerDeck cardsInside;
    unsigned int gameValue;
    double parentProbability;
    double selfProbability;
    TreeNode(PlayerDeck cards, unsigned int width,double parentProbability);

};

struct GameDeck:public Deck
{
    unsigned int deckMultiplier;
    void createDebugDeck();
    void createLargeDeck(int deckMultiplier);
    void resetDeck(int deckMultiplier);
    void removeCardsForTreeOperation(TreeNode node);
    void shuffle();
    GameDeck(unsigned int deckMultiplier);
};

struct Player{

    unsigned int gameValue;
    unsigned int openCardValue;
    unsigned int numberOfCards;
    unsigned int totalValue;
    unsigned int roundScore = 0;
    unsigned int simulationScore = 0;
    PlayerDeck cardsInsideHand;
    void printSubjectCards();
    unsigned getPlayerOpenCardValue();
    unsigned getPlayerGameValue();
    std::vector<int> getCards();
    unsigned int getNumberOfUnknownCards();
    void clearHand();
    void drawSpecificCard(unsigned int cardToDraw,GameDeck &actualDeck);
    void addRoundScore();
};

struct ProbBar
{
    double probArray[6];

    ProbBar();
    void addProb(double prob,unsigned int deckValue);
    void printBar();
    void clearBar();
    double getWinProb(unsigned int handValue);

};

struct Glados:public Player
{
    double expectedValueOfDrawingCard;
    double currentWinProb;
    ProbBar treeFunction(GameDeck originalDeck, unsigned int openCardValue);
    void updateExpectedValue(GameDeck originalDeck, unsigned int openCardValue);
    unsigned int getImaginaryHandValue(unsigned int imaginaryCardToDraw);
    void drawSpecificCard(unsigned int cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck);
};


#ifndef BLACKJACK_REAL_CLASSES_H
#define BLACKJACK_REAL_CLASSES_H

#endif //BLACKJACK_REAL_CLASSES_H
