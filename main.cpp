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

    GameDeck actualDeck(1);
    GameDeck knownDeck(1);

    actualDeck.createLargeDeck(1);
    knownDeck.createLargeDeck(1);

    Glados glados;
    Player dealer;


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

        actualDeck.printCards();
        knownDeck.printCards();

        std::cout<<"And what's the dealers open card?"<<"\n";
        std::cin>>dealerOpenCard;

        dealer.drawSpecificCard(dealerOpenCard,actualDeck);

        knownDeck.removeCard(dealer.getPlayerOpenCardValue());

        actualDeck.printCards();
        knownDeck.printCards();

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

        glados.clearHand();
        dealer.clearHand();

        knownDeck.equalizeDeck(actualDeck);

    }

    std::cout<<"Glados: "<<glados.roundScore<<"  "<<"Dealer: "<<dealer.roundScore<<"\n";


    return 0;
}
