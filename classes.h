//
// Created by yagiz on 1/29/2024.
//
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "iostream"
#include <map>
#include <random>
#include "fstream"

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

    unsigned int getNumberOfAces();
    unsigned int getOpenCardValue();
    unsigned int getTotalValue();
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
    int getRandomCard();
    GameDeck(unsigned int deckMultiplier);
};

struct Player{


    unsigned int roundScore = 0;
    unsigned int simulationScore = 0;
    PlayerDeck cardsInsideHand;
    void printSubjectCards();
    void writeSubjectCardsTxt(std::fstream &fileToWrite);
    unsigned getPlayerOpenCardValue();
    unsigned getPlayerGameValue();
    std::vector<int> getCards();
    unsigned int getNumberOfUnknownCards();
    void clearHand();
    void drawSpecificCard(unsigned int cardToDraw,GameDeck &actualDeck);
    void addRoundScore();
    void drawImaginaryCard(unsigned int cardToDraw);
    void drawRandomCard(GameDeck &actualDeck);
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
    void drawRandomCard(GameDeck &actualDeck, GameDeck &knownDeck);
    unsigned int getLastCard();
};

struct DealerCopycat: public Player
{
    void drawGhostCard(unsigned int cardToAdd);
    void returnTheCards(GameDeck &actualDeck);
    bool canMirrorCard(Glados glados);
    void mirrorCard(Glados glados);
};

class Table
{
    Player dealer;
    Glados glados;
    DealerCopycat copycat;
    std::fstream simResults;



public:
    Table();
    void startNormalGame();
    void startSimulation(unsigned int roundToWin, unsigned int simulationToWin);

};


#ifndef BLACKJACK_REAL_CLASSES_H
#define BLACKJACK_REAL_CLASSES_H

#endif //BLACKJACK_REAL_CLASSES_H
