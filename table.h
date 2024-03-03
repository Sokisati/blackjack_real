//
// Created by yagiz on 2/22/2024.
//



#ifndef BLACKJACK_REAL_TABLE_H
#define BLACKJACK_REAL_TABLE_H

#include "player.h"
#include "fstream"

class Table
{
public:
    Dealer dealer;
    Glados glados;
    DealerCopycat copycat;
    std::fstream resultTxt;
    GameDeck actualDeck;
    GameDeck knownDeck;
    GameDeck ghostDeck;
    GameDeck actualDeckCopy;
    int copycatIndex = 0;
    unsigned int copycatRepetition;


    Table(unsigned int deckMultiplier, unsigned int copycatRepetition);
    void startNormalGame();
    void startSimulation(unsigned int roundToWin, unsigned int simulationToWin);
    void dealCards();
    void endRound(unsigned int roundToWin);
    void startRound();
    void writeResultsToTxt();
    void printHands();
    int gladosVCopycat(unsigned int gladosGameValue, unsigned int dealerGameValue, unsigned int copycatGameValue);
    void updateComplexCopycatIndex(unsigned int repetition, const GameDeck& actualDeckOriginal, Glados gladosClone, Dealer dealerClone, DealerCopycat copycatClone);

};

#endif //BLACKJACK_REAL_TABLE_H
