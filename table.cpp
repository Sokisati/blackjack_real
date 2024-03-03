//
// Created by yagiz on 2/22/2024.
//

#include "table.h"
#include "player.h"
#include "iostream"

Table::Table(unsigned int deckMultiplier, unsigned int copycatRepetition) : actualDeck(deckMultiplier), knownDeck(deckMultiplier), ghostDeck(deckMultiplier),
                                            actualDeckCopy(deckMultiplier)
{
    this->copycatRepetition = copycatRepetition;
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
        glados.drawCardBasedOnExpectedValue(actualDeck,knownDeck,dealerOpenCard);
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

void Table::printHands()
{
    std::cout<<"Glados hand: ";
    glados.printSubjectCards();
    std::cout<<"Dealer hand: ";
    dealer.printSubjectCards();
    std::cout<<"Copycat hand:";
    copycat.printSubjectCards();
}

void Table::dealCards()
{
    glados.drawRandomCard(actualDeck,knownDeck);
    glados.drawRandomCard(actualDeck,knownDeck);
    dealer.drawRandomCard(actualDeck);
    dealer.drawRandomCard(actualDeck);
    copycat.cardsInsideHand.equalizeDeck(glados.cardsInsideHand);

    knownDeck.removeCard(dealer.getPlayerOpenCardValue());

    if(actualDeck.getNumberOfCards()+1!=knownDeck.getNumberOfCards())
    {
        throw AsyncDecks();
    }
    else if(actualDeck.getNumberOfCards()==0)
    {
        throw OutOfCards();
    }
}

int Table::gladosVCopycat(unsigned int gladosGameValue, unsigned int dealerGameValue, unsigned int copycatGameValue)
{

        if(gladosGameValue>dealerGameValue&&(copycatGameValue<dealerGameValue||copycatGameValue==dealerGameValue))
        {
            return 1;
        }
        else if(copycatGameValue>dealerGameValue&&(gladosGameValue<dealerGameValue||gladosGameValue==dealerGameValue))
        {
            return -1;
        }
        else if(gladosGameValue==dealerGameValue&&copycatGameValue<dealerGameValue)
        {
            return 1;
        }
        else if(copycatGameValue==dealerGameValue&&gladosGameValue<dealerGameValue)
        {
            return -1;
        }
        else
        {
            return 0;
        }
}

void Table::endRound(unsigned int roundToWin)
{


    printHands();
    std::cout<<"cc index: "<<copycatIndex<<"\n";

    if(glados.getPlayerGameValue()>dealer.getPlayerGameValue())
    {
        glados.addRoundScore();
    }
    else if(glados.getPlayerGameValue()<dealer.getPlayerGameValue())
    {
        dealer.addRoundScore();
    }




    glados.clearHand();
    dealer.clearHand();
    copycat.clearHand();
    knownDeck.equalizeDeck(actualDeck);

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


    std::cout<<"\n";
}

void Table::writeResultsToTxt()
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

void Table::startRound()
{
    actualDeck.printCards();
    actualDeckCopy.equalizeDeck(actualDeck);
    glados.clearHand();
    dealer.clearHand();
    copycat.clearHand();
    knownDeck.equalizeDeck(actualDeck);
    dealCards();
}

void Table::updateComplexCopycatIndex(unsigned int repetition, const GameDeck& actualDeckOriginal, Glados gladosClone, Dealer dealerClone, DealerCopycat copycatClone)
{
    //this is definitely the worst method in this program, don't think too much about it

    unsigned int dealerFirstCard = dealerClone.getPlayerOpenCardValue();
    unsigned int gladosFirstCard = gladosClone.cardsInsideHand.getElement(0);
    unsigned int gladosSecondCard = gladosClone.cardsInsideHand.getElement(1);

    std::cout<<"inside"<<"\n";

    bool drawNot = false;
    bool drawMany = false;

    if(gladosClone.cardsInsideHand.getNumberOfCards()==2)
    {
        drawNot=true;
    }
    else
    {
        drawMany=true;
    }

    int result = 0;

    GameDeck actualDeckClone(1);
    GameDeck knownDeckClone(1);

    actualDeckClone.equalizeDeck(actualDeckOriginal);

    //test for glados
    for(int i=0; i<repetition; i++)
    {
        gladosClone.clearHand();
        dealerClone.clearHand();
        actualDeckClone.equalizeDeck(actualDeckOriginal);
        knownDeckClone.equalizeDeck(actualDeckClone);

       gladosClone.drawSpecificCard(gladosFirstCard,actualDeckClone,knownDeckClone);
       gladosClone.drawSpecificCard(gladosSecondCard,actualDeckClone,knownDeckClone);
       dealerClone.drawSpecificCard(dealerFirstCard,actualDeckClone);

       knownDeckClone.removeCard(dealerFirstCard);

       if(drawMany)
       {
           gladosClone.drawRandomCard(actualDeckClone,knownDeckClone);
           try
           {
               gladosClone.drawCardBasedOnExpectedValue(actualDeckClone,knownDeckClone,dealerFirstCard);
           }
           catch(TreeGenerationError &err)
           {
               std::cout<<err.what()<<"\n";
               actualDeck.resetDeck();
               i--;
               continue;
           }
           catch(OutOfCards &err)
           {
               std::cout<<err.what()<<"\n";
               actualDeck.resetDeck();
               i--;
               continue;
           }
       }

       try
       {
           dealerClone.drawCardSoft17(actualDeckClone);
       }
       catch(OutOfCards &err)
       {
           std::cout<<err.what()<<"\n";
           actualDeck.resetDeck();
           i--;
           continue;
       }

        if(gladosClone.getPlayerGameValue()>dealerClone.getPlayerGameValue())
        {
            result++;
        }
        else if(gladosClone.getPlayerGameValue()<dealerClone.getPlayerGameValue())
        {
            result--;
        }

        gladosClone.printSubjectCards();
        dealerClone.printSubjectCards();
        std::cout<<"result: "<<result<<"\n";

    }

    for(int i=0; i<repetition; i++)
    {
        copycatClone.clearHand();
        dealerClone.clearHand();

        actualDeckClone.equalizeDeck(actualDeckOriginal);

       copycatClone.drawSpecificCard(gladosFirstCard,actualDeckClone);
       copycatClone.drawSpecificCard(gladosSecondCard,actualDeckClone);
       dealerClone.drawSpecificCard(dealerFirstCard,actualDeckClone);

       try
       {
           copycatClone.drawCardSoft17(actualDeckClone);
           dealerClone.drawCardSoft17(actualDeckClone);
       }
       catch(OutOfCards &err)
       {
           std::cout<<err.what()<<"\n";
           actualDeck.resetDeck();
           i--;
           continue;
       }
        if(copycatClone.getPlayerGameValue()>dealerClone.getPlayerGameValue())
        {
            result--;
        }
        else if(copycatClone.getPlayerGameValue()<dealerClone.getPlayerGameValue())
        {
            result++;
        }

        copycatClone.printSubjectCards();
        dealerClone.printSubjectCards();
        std::cout<<"result: "<<result<<"\n";
    }

    if(result>0)
    {
        copycatIndex++;
    }
    else if(result<0)
    {
        copycatIndex--;
    }

}

void Table::startSimulation(unsigned int roundToWin, unsigned int simulationToWin)
{
    while(glados.simulationScore<simulationToWin && dealer.simulationScore<simulationToWin)
    {

        try
        {
            startRound();
        }
        catch(OutOfCards &err)
        {
            std::cout<<err.what()<<"\n";
            actualDeck.resetDeck();
            continue;
        }


        ghostDeck.equalizeDeck(actualDeck);

        try
        {
            glados.drawCardBasedOnExpectedValue(actualDeck,knownDeck,dealer.getPlayerOpenCardValue());
        }
        catch(TreeGenerationError &err)
        {
            std::cout<<err.what()<<"\n";
            actualDeck.resetDeck();
            continue;
        }
        catch(OutOfCards &err)
        {
            std::cout<<err.what()<<"\n";
            actualDeck.resetDeck();
            continue;
        }

        try
        {
            copycat.drawCardLikeDealer(glados,ghostDeck);
        }
        catch(TreeGenerationError &err)
        {
            std::cout<<err.what()<<"\n";
            actualDeck.resetDeck();
            continue;
        }
        catch(OutOfCards &err)
        {
            std::cout<<err.what()<<"\n";
            actualDeck.resetDeck();
            continue;
        }

        try
        {
            dealer.drawCardSoft17(actualDeck);
        }
        catch(OutOfCards &err)
        {
            std::cout<<err.what()<<"\n";
            actualDeck.resetDeck();
            continue;
        }

        if(glados.cardsInsideHand.getNumberOfCards()!=copycat.cardsInsideHand.getNumberOfCards())
        {
            updateComplexCopycatIndex(copycatRepetition,actualDeckCopy,glados,dealer,copycat);
        }


        endRound(roundToWin);

        std::cout<<"\n\n";
    }
    std::cout<<"glados: "<<glados.simulationScore<<" "<<" dealer:"<<dealer.simulationScore<<"\n";

    resultTxt.close();
}