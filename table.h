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


    Table();
    void startNormalGame();
    void startSimulation(unsigned int roundToWin, unsigned int simulationToWin);
    void dealCards(Glados &glados, Dealer &dealer, GameDeck &actualDeck, GameDeck &knownDeck);
    void endRound(GameDeck &actualDeck, GameDeck &knownDeck);
    void writeResultsToTxt(GameDeck gameDeck);

};

#endif //BLACKJACK_REAL_TABLE_H
