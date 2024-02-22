//
// Created by yagiz on 2/22/2024.
//



#ifndef BLACKJACK_REAL_TABLE_H
#define BLACKJACK_REAL_TABLE_H

#include "player.h"

class Table
{
public:
    Dealer dealer;
    Glados glados;
    DealerCopycat copycat;
    int copycatIndex = 0;

    void startNormalGame();
    void startSimulation(unsigned int roundToWin, unsigned int simulationToWin);
    void dealCards(Glados &glados, Dealer &dealer, GameDeck &actualDeck, GameDeck &knownDeck);
    void endRound();

};

#endif //BLACKJACK_REAL_TABLE_H
