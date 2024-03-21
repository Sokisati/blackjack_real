//
// Created by yagiz on 2/22/2024.
//

#include "table.h"
#include "player.h"
#include "iostream"

Table::Table(unsigned int deckMultiplier) : actualDeck(deckMultiplier), knownDeck(deckMultiplier), ghostDeck(deckMultiplier),
                                            actualDeckCopy(deckMultiplier)
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
        knownDeck.removeCard(dealer.getPlayerOpenCard());
        expectedValue = glados.getExpectedValue(knownDeck,dealer.getPlayerOpenCard());
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
        knownDeck.copyDeck(actualDeck);
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

    resultTxt<<"Glados hand: ";
    glados.cardsInsideHand.writeCards(resultTxt);
    resultTxt<<"Dealer hand: ";
    dealer.cardsInsideHand.writeCards(resultTxt);
    resultTxt<<"Copycat hand:";
    copycat.cardsInsideHand.writeCards(resultTxt);

}

void Table::dealCards()
{
    glados.drawRandomCard(actualDeck,knownDeck);
    glados.drawRandomCard(actualDeck,knownDeck);
    dealer.drawRandomCard(actualDeck);
    dealer.drawRandomCard(actualDeck);
    copycat.cardsInsideHand.copyDeck(glados.cardsInsideHand);

    knownDeck.removeCard(dealer.getPlayerOpenCard());

    if(actualDeck.getNumberOfCards()+1!=knownDeck.getNumberOfCards())
    {
        throw AsyncDecks();
    }
    else if(actualDeck.getNumberOfCards()==0)
    {
        throw OutOfCards();
    }
}

void Table::endRound(unsigned int roundToWin)
{
    printHands();

    std::cout<<"cc index: "<<copycatIndex<<"\n";
    resultTxt<<"cc index: "<<copycatIndex<<"\n";

    if(glados.getPlayerGameValue()==0)
    {
        dealer.addRoundScore();
    }
    else
    {
        if(glados.getPlayerGameValue()>dealer.getPlayerGameValue())
        {
            glados.addRoundScore();
        }
        else if(glados.getPlayerGameValue()<dealer.getPlayerGameValue())
        {
            dealer.addRoundScore();
        }
    }

    glados.clearHand();
    dealer.clearHand();
    copycat.clearHand();
    knownDeck.copyDeck(actualDeck);

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

    std::cout<<"glados rs: "<<glados.roundScore<<" dealer rs: "<<dealer.roundScore<<"\n";
    std::cout<<"glados ss: "<<glados.simulationScore<<" dealer ss: "<<dealer.simulationScore<<"\n";
    std::cout<<"\n";
    resultTxt<<"\n";
}

void Table::startRound()
{
    actualDeck.printCards();
    actualDeck.writeCards(resultTxt);
    actualDeckCopy.copyDeck(actualDeck);
    glados.clearHand();
    dealer.clearHand();
    copycat.clearHand();
    knownDeck.copyDeck(actualDeck);
    dealCards();
}

void Table::updateComplexCopycatIndex(unsigned int repetition, const GameDeck& actualDeckOriginal, Glados gladosClone, Dealer dealerClone, DealerCopycat copycatClone)
{
    //this is definitely the worst method in this program, don't think too much about it

    unsigned int dealerFirstCard = dealerClone.getPlayerOpenCard();
    unsigned int gladosFirstCard = gladosClone.cardsInsideHand.getElement(0);
    unsigned int gladosSecondCard = gladosClone.cardsInsideHand.getElement(1);

    std::cout<<"inside"<<"\n";
    bool drawMany = false;

    if(gladosClone.cardsInsideHand.getNumberOfCards()>2)
    {
        drawMany=true;
    }
    int result = 0;
    GameDeck actualDeckClone(1);
    GameDeck knownDeckClone(1);
    actualDeckClone.copyDeck(actualDeckOriginal);
    //for glados
    for(int i=0; i<repetition; i++)
    {
        gladosClone.clearHand();
        dealerClone.clearHand();
        actualDeckClone.copyDeck(actualDeckOriginal);
        knownDeckClone.copyDeck(actualDeckClone);

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

       if(gladosClone.getPlayerGameValue()==0)
       {
           result--;
       }
       else
       {
           if(gladosClone.getPlayerGameValue()>dealerClone.getPlayerGameValue())
           {
               result++;
           }
           else if(gladosClone.getPlayerGameValue()<dealerClone.getPlayerGameValue())
           {
               result--;
           }
       }
        gladosClone.printSubjectCards();
        dealerClone.printSubjectCards();
        std::cout<<"result: "<<result<<"\n";

    }

    //for copycat
    for(int i=0; i<repetition; i++)
    {
        copycatClone.clearHand();
        dealerClone.clearHand();

        actualDeckClone.copyDeck(actualDeckOriginal);

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

       if(copycatClone.getPlayerGameValue()==0)
       {
           result++;
       }
       else
       {
           if(copycatClone.getPlayerGameValue()>dealerClone.getPlayerGameValue())
           {
               result--;
           }
           else if(copycatClone.getPlayerGameValue()<dealerClone.getPlayerGameValue())
           {
               result++;
           }
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

void Table::startSimulation(unsigned int roundToWin, unsigned int simulationToWin,unsigned int scenarioRepetition)
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

        ghostDeck.copyDeck(actualDeck);
        try
        {
            glados.drawCardBasedOnExpectedValue(actualDeck,knownDeck,dealer.getPlayerOpenCard());
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
            updateComplexCopycatIndex(scenarioRepetition,actualDeckCopy,glados,dealer,copycat);
        }


        endRound(roundToWin);

        std::cout<<"\n\n";
        resultTxt<<"\n\n";
    }
    std::cout<<"glados: "<<glados.simulationScore<<" "<<" dealer:"<<dealer.simulationScore<<"\n";
    resultTxt<<"glados: "<<glados.simulationScore<<" "<<" dealer:"<<dealer.simulationScore<<"\n";

    resultTxt.close();
}