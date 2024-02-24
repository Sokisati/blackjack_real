//
// Created by yagiz on 2/22/2024.
//

#include "table.h"
#include "player.h"
#include "iostream"

Table::Table()
{
   resultTxt.open("test_results.txt",std::ios::out);
}

void Table::startNormalGame()
{
    unsigned int firstInitialCard;
    unsigned int secondInitialCard;
    unsigned int dealerOpenCard;
    unsigned int gladosExtraCard;
    unsigned int dealerSecretCard;
    unsigned int copycatExtraCard;
    double expectedValue;

    GameDeck actualDeck(1);
    GameDeck knownDeck(1);

    actualDeck.createLargeDeck();
    knownDeck.createLargeDeck();


    while(true)
    {
        //deal the cards
        std::cout<<"What are my cards?"<<"\n";
        std::cin>>firstInitialCard;
        std::cin>>secondInitialCard;
        //exit condition
        if(firstInitialCard==999||secondInitialCard==999)
        {
            break;
        }
        glados.drawSpecificCard(firstInitialCard,actualDeck,knownDeck);
        glados.drawSpecificCard(secondInitialCard,actualDeck,knownDeck);
        std::cout<<"And what's the dealers open card?"<<"\n";
        std::cin>>dealerOpenCard;
        dealer.drawSpecificCard(dealerOpenCard,actualDeck);
        knownDeck.removeCard(dealer.getPlayerOpenCardValue());
        expectedValue = glados.getExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());
        while(expectedValue>0)
        {
            std::cout<<"I want a card. Take one for me"<<"\n";
            std::cin>>gladosExtraCard;
            glados.drawSpecificCard(gladosExtraCard,actualDeck,knownDeck);
            expectedValue = glados.getExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());
        }
        std::cout<<"I don't want any/more cards"<<"\n";
        while(true)
        {
            std::cout<<"Reveal card of dealer"<<"\n";
            std::cin>>dealerSecretCard;
            dealer.drawSpecificCard(dealerSecretCard,actualDeck);
            if(dealer.getPlayerGameValue()>=17 || dealer.getPlayerGameValue()==0)
            {
                break;
            }
        }
        if(glados.getPlayerGameValue()>dealer.getPlayerGameValue())
        {
            std::cout<<"I won, as expected."<<"\n";
            glados.addRoundScore();
        }
        else if(glados.getPlayerGameValue()<dealer.getPlayerGameValue())
        {
            std::cout<<"You must have cheated or smth."<<"\n";
            dealer.addRoundScore();
        }
        else
        {
            std::cout<<"It's a draw. I hate draws."<<"\n";
        }
        glados.clearHand();
        dealer.clearHand();
        knownDeck.equalizeDeck(actualDeck);
        knownDeck.printCards();
        actualDeck.printCards();

    }

    std::cout<<"glados: "<<glados.roundScore<<" "<<" dealer:"<<dealer.roundScore<<"\n";

}

void Table::dealCards(Glados &glados, Dealer &dealer, GameDeck &actualDeck, GameDeck &knownDeck)
{
    glados.drawRandomCard(actualDeck,knownDeck);
    glados.drawRandomCard(actualDeck,knownDeck);
    dealer.drawRandomCard(actualDeck);
    dealer.drawRandomCard(actualDeck);
    knownDeck.removeCard(dealer.getPlayerOpenCardValue());
}

void Table::endRound()
{
    if(glados.getPlayerGameValue()>dealer.getPlayerGameValue())
    {
        if(copycat.getPlayerGameValue()<dealer.getPlayerGameValue() || copycat.getPlayerGameValue()==dealer.getPlayerGameValue())
        {
            copycatIndex++;
        }
        glados.addRoundScore();
    }
    else if(glados.getPlayerGameValue()<dealer.getPlayerGameValue())
    {
        if(copycat.getPlayerGameValue()>dealer.getPlayerGameValue()||copycat.getPlayerGameValue()==dealer.getPlayerGameValue())
        {
            copycatIndex--;
        }
        dealer.addRoundScore();
    }
    else
    {
        if(copycat.getPlayerGameValue()<dealer.getPlayerGameValue())
        {
            copycatIndex++;
        }
        else if(copycat.getPlayerGameValue()>dealer.getPlayerGameValue())
        {
            copycatIndex--;
        }
    }

    std::cout<<"\n";
}

void Table::writeResultsToTxt(GameDeck knownDeck)
{

    resultTxt<<"Glados hand: ";
    for(unsigned int card: glados.cardsInsideHand.cards)
    {
        resultTxt<<card<<" ";
    }
    resultTxt<<"\n";

    resultTxt<<"Dealer hand: ";
    for(unsigned int card: dealer.cardsInsideHand.cards)
    {
        resultTxt<<card<<" ";
    }
    resultTxt<<"\n";

    resultTxt<<"Copycat hand: ";
    for(unsigned int card: copycat.cardsInsideHand.cards)
    {
        resultTxt<<card<<" ";
    }
    resultTxt<<"\n";

    resultTxt<<"copycat index: "<<copycatIndex<<"\n";

    if(copycat.getCards()!=glados.getCards())
    {
        for(unsigned int card: knownDeck.cards)
        {
            resultTxt<<card<<" ";
        }
        resultTxt<<"\n";
    }

    resultTxt<<"\n";
    resultTxt<<"\n";

}

void Table::startSimulation(unsigned int roundToWin, unsigned int simulationToWin)
{

    GameDeck actualDeck(1);
    GameDeck knownDeck(1);

    GameDeck ghostDeck(1);

    GameDeck knownDeckOriginal(1);

    actualDeck.createLargeDeck();
    knownDeck.createLargeDeck();

    while(glados.simulationScore<simulationToWin && dealer.simulationScore<simulationToWin)
    {

        actualDeck.printCards();
        if(actualDeck.getNumberOfCards()<5)
        {
            actualDeck.createLargeDeck();
            knownDeck.createLargeDeck();
            actualDeck.printCards();
        }

        dealCards(glados,dealer,actualDeck,knownDeck);

        knownDeckOriginal.equalizeDeck(knownDeck);

        copycat.cardsInsideHand.equalizeDeck(glados.cardsInsideHand);

        ghostDeck.equalizeDeck(actualDeck);

        try
        {
            glados.drawCardBasedOnExpectedValue(actualDeck,knownDeck,dealer.getPlayerOpenCardValue());
        }
        catch(TreeGenerationError &err)
        {
            std::cout<<err.what()<<"\n";

            glados.clearHand();
            dealer.clearHand();
            copycat.clearHand();

            actualDeck.createLargeDeck();
            knownDeck.createLargeDeck();

            dealCards(glados,dealer,actualDeck,knownDeck);
            knownDeckOriginal.equalizeDeck(knownDeck);

            copycat.cardsInsideHand.equalizeDeck(glados.cardsInsideHand);

            ghostDeck.equalizeDeck(actualDeck);
        }


        copycat.drawCardLikeDealer(glados,ghostDeck);

        dealer.drawCardSoft17(actualDeck,knownDeck);

        std::cout<<"Glados hand: ";
        glados.printSubjectCards();
        std::cout<<"Dealer hand: ";
        dealer.printSubjectCards();
        std::cout<<"Copycat hand:";
        copycat.printSubjectCards();

        writeResultsToTxt(knownDeckOriginal);

        endRound();

        glados.clearHand();
        dealer.clearHand();
        copycat.clearHand();
        knownDeck.equalizeDeck(actualDeck);
        ghostDeck.equalizeDeck(actualDeck);

        std::cout<<"gr: "<<glados.roundScore<<" dr:"<<dealer.roundScore<<"\n";
        std::cout<<"gs: "<<glados.simulationScore<<" ds: "<<dealer.simulationScore<<"\n";
        std::cout<<"cc: "<<copycatIndex<<"\n";

        if(glados.roundScore==roundToWin)
        {
            glados.simulationScore++;
            glados.roundScore=0;
            dealer.roundScore=0;
        }
        else if(dealer.roundScore==roundToWin)
        {
            dealer.simulationScore++;
            glados.roundScore=0;
            dealer.roundScore=0;
        }

        std::cout<<"\n\n";
    }
    std::cout<<"glados: "<<glados.simulationScore<<" "<<" dealer:"<<dealer.simulationScore<<"\n";
    std::cout<<"cc: "<<copycatIndex<<"\n";

    resultTxt.close();
}