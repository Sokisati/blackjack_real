#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
#include "classes.h"

int main()
{

    unsigned int firstInitialCard;
    unsigned int secondInitialCard;
    unsigned int dealerOpenCard;
    unsigned int gladosExtraCard;
    unsigned int dealerSecretCard;
    unsigned int copycatExtraCard;

    unsigned int copycatScore;

    GameDeck actualDeck(1);
    GameDeck knownDeck(1);

    actualDeck.createLargeDeck(1);
    knownDeck.createLargeDeck(1);

    Glados glados;
    Player dealer;
    DealerCopycat copycat;

    /*
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

        //give the same cards glados has
        copycat.cardsInsideHand.equalizeDeck(glados.cardsInsideHand);

        std::cout<<"And what's the dealers open card?"<<"\n";
        std::cin>>dealerOpenCard;

        dealer.drawSpecificCard(dealerOpenCard,actualDeck);

        knownDeck.removeCard(dealer.getPlayerOpenCardValue());

        while(true)
        {
            std::cout<<"Draw card for copycat"<<"\n";
            std::cin>>copycatExtraCard;
            copycat.drawGhostCard(copycatExtraCard);
            copycat.printSubjectCards();
            if(copycat.getPlayerGameValue()>=17 || copycat.getPlayerGameValue()==0)
            {
                break;
            }
        }

        std::cout<<"Now, in the same order; put those cards back to the deck"<<"\n";

        glados.updateExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());

        while(glados.expectedValueOfDrawingCard>=0)
        {
            std::cout<<"I want a card. Take one for me"<<"\n";
            std::cin>>gladosExtraCard;
            glados.drawSpecificCard(gladosExtraCard,actualDeck,knownDeck);
            glados.updateExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());
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

        if(copycat.getPlayerGameValue()>dealer.getPlayerGameValue())
        {
            copycat.addRoundScore();
        }

        knownDeck.printCards();
        actualDeck.printCards();

        glados.clearHand();
        dealer.clearHand();
        copycat.clearHand();

        knownDeck.equalizeDeck(actualDeck);

    }
*/

    actualDeck.createDebugDeck();
    knownDeck.createDebugDeck();

    glados.drawImaginaryCard(10);
    glados.drawImaginaryCard(5);

    dealer.drawImaginaryCard(8);
    dealer.drawImaginaryCard(9);

    glados.updateExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());

    std::cout<<"Glados: "<<glados.roundScore<<"  "<<"Dealer: "<<dealer.roundScore<<"\n";

    return 0;
}
