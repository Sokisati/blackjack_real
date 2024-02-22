//
// Created by yagiz on 2/22/2024.
//
#include "tree_and_bar.h"
#include "iostream"


ProbBar::ProbBar()
{
    for(int i=0; i<6; i++)
    {
        probArray[i] = 0;
    }
}

void ProbBar::addProb(double prob,unsigned int deckValue)
{
    switch (deckValue)
    {
        case 0:
            probArray[0] += prob;
            break;
        case 17:
            probArray[1] += prob;
            break;
        case 18:
            probArray[2] += prob;
            break;
        case 19:
            probArray[3] += prob;
            break;
        case 20:
            probArray[4] += prob;
            break;
        case 21:
            probArray[5] += prob;
            break;
    }
}

void ProbBar::printBar()
{
    std::cout<<"0: "<<probArray[0]<<"\n";
    for(int i=1; i<6; i++)
    {
        std::cout<<i+16<<" "<<probArray[i]<<"\n";
    }
}

double ProbBar::getWinProb(unsigned int handValue)
{
    double sum = 0;
    if(handValue<17)
    {
        sum = probArray[0];
    }
    else
    {
        for(int i=0; i<=handValue-16; i++)
        {
            sum += probArray[i];
        }
    }
    return sum;
}

void ProbBar::clearBar()
{
    for(double & i : probArray)
    {
        i = 0;
    }
}


TreeNode::TreeNode(PlayerDeck cards,unsigned int width,double parentProbability)
{
    this->cardsInside = cards;
    this->parentProbability = parentProbability;
    this->selfProbability = parentProbability * 1/(static_cast<double>(width));
}

