//
// Created by yagiz on 2/22/2024.
//

#include "table.h"
#include "player.h"
#include "iostream"

Table::Table(unsigned int deckMultiplier,unsigned int deckDepletedAssumption) : actualDeck(deckMultiplier), knownDeck(deckMultiplier), ghostDeck(deckMultiplier),
                                            actualDeckCopy(deckMultiplier)
{
    this->deckDepletedAssumption = deckDepletedAssumption;
   resultTxt.open("test_results.txt",std::ios::out);
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
    if(actualDeck.getNumberOfCards()<deckDepletedAssumption)
    {
        actualDeck.resetDeck();
        knownDeck.resetDeck();
    }

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

    card_t dealerFirstCard = dealerClone.getPlayerOpenCard();
    card_t gladosFirstCard = gladosClone.cardsInsideHand.getElement(0);
    card_t gladosSecondCard = gladosClone.cardsInsideHand.getElement(1);

    std::cout<<"inside"<<"\n";
    bool drawMany = false;

    double expectedValue;

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
           expectedValue = gladosClone.getExpectedValue(knownDeckClone,dealerClone.getPlayerOpenCard());

           while(expectedValue>0)
           {
               if(gladosClone.getPlayerGameValue()==0 || gladosClone.getPlayerGameValue()>=19 ||actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
               {
                   break;
               }
               gladosClone.drawRandomCard(actualDeckClone,knownDeckClone);
               expectedValue = gladosClone.getExpectedValue(knownDeckClone,dealerClone.getPlayerOpenCard());
           }
           if(actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
           {
               return;
           }
       }

        //dealer cycle
        while(true)
        {
            if(dealerClone.getPlayerGameValue()>=17 || dealerClone.getPlayerGameValue()==0 || actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
            {
                break;
            }
            else
            {
                dealerClone.drawRandomCard(actualDeckClone);
            }
        }
        if(actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
        {
            return;
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

       //copycat cycle
        while(true)
        {
            if(copycatClone.getPlayerGameValue()>=17 || copycatClone.getPlayerGameValue()==0 || actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
            {
                break;
            }
            else
            {
                copycatClone.drawRandomCard(actualDeckClone);
            }
        }
        if(actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
        {
            return;
        }

        //dealer cycle
        while(true)
        {
            if(dealerClone.getPlayerGameValue()>=17 || dealerClone.getPlayerGameValue()==0 || actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
            {
                break;
            }
            else
            {
                dealerClone.drawRandomCard(actualDeckClone);
            }
        }
        if(actualDeckClone.getNumberOfCards()<deckDepletedAssumption)
        {
            return;
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
    double expectedValue;

    while(glados.simulationScore<simulationToWin && dealer.simulationScore<simulationToWin)
    {

        startRound();

        ghostDeck.copyDeck(actualDeck);

        expectedValue = glados.getExpectedValue(knownDeck,dealer.getPlayerOpenCard());

        //glados cycle
        while(expectedValue>0)
        {
            if(glados.getPlayerGameValue()==0 ||glados.getPlayerGameValue()>=19 ||actualDeck.getNumberOfCards()<deckDepletedAssumption)
            {
                break;
            }

            glados.drawRandomCard(actualDeck,knownDeck);
            expectedValue = glados.getExpectedValue(knownDeck,dealer.getPlayerOpenCard());
        }

        if(actualDeck.getNumberOfCards()<deckDepletedAssumption)
        {
            continue;
        }


        //copycat cycle
        while(true)
        {
            if(copycat.getPlayerGameValue()==0 || copycat.getPlayerGameValue()>=17 ||ghostDeck.getNumberOfCards()<deckDepletedAssumption)
            {
                break;
            }

            if(copycat.canMirrorCard(glados))
            {
                copycat.mirrorCard(glados);
            }
            else
            {
                copycat.drawRandomCard(ghostDeck);
            }
        }

        if(ghostDeck.getNumberOfCards()<deckDepletedAssumption)
        {
            continue;
        }

        //dealer cycle
        while(true)
        {
            if(dealer.getPlayerGameValue()>=17 || dealer.getPlayerGameValue()==0 || actualDeck.getNumberOfCards()<deckDepletedAssumption)
            {
                break;
            }
            else
            {
                dealer.drawRandomCard(actualDeck);
            }
        }

        if(actualDeck.getNumberOfCards()<deckDepletedAssumption)
        {
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