//
// Created by yagiz on 2/22/2024.
//

#ifndef BLACKJACK_REAL_TREE_AND_BAR_H
#define BLACKJACK_REAL_TREE_AND_BAR_H

#include "deck.h"
#include <exception>

typedef unsigned int handvalue_t;

struct TreeGenerationError : public std::exception
{
    const char* what();
};

struct TreeNode
{
    PlayerDeck cardsInside;
    double parentProbability;
    double selfProbability;
    TreeNode(PlayerDeck cards, unsigned int width,double parentProbability);

};

struct ProbBar
{
    double probArray[6];

    ProbBar();
    void addProb(double prob,handvalue_t handValue);
    void printBar();
    void clearBar();
    double getWinProb(handvalue_t handValue);

};
#endif //BLACKJACK_REAL_TREE_AND_BAR_H
