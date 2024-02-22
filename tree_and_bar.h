//
// Created by yagiz on 2/22/2024.
//

#ifndef BLACKJACK_REAL_TREE_AND_BAR_H
#define BLACKJACK_REAL_TREE_AND_BAR_H

#include "deck.h"

struct TreeNode
{
    PlayerDeck cardsInside;
    unsigned int gameValue;
    double parentProbability;
    double selfProbability;
    TreeNode(PlayerDeck cards, unsigned int width,double parentProbability);

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
#endif //BLACKJACK_REAL_TREE_AND_BAR_H
