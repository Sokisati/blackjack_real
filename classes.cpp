//
// Created by yagiz on 1/29/2024.
//
#include "classes.h"
#include "iostream"
#include "vector"
#include <cstdlib>
#include <map>

void GameDeck::createDebugDeck()
{
    cards.clear();
    cards.push_back(8);
    cards.push_back(3);
    cards.push_back(7);
    cards.push_back(10);
    cards.push_back(11);
    cards.addCard(6);//Use the addCard method somehow, don't meddle with arr/vector attributes directly

}

unsigned int PlayerDeck::calculateTotalValue()
{
    unsigned int sum = 0;
    for(auto i: cards)
    {
        sum += i;
    }
    return sum;
}

unsigned int Deck::getNumberOfCards()
{
    return cards.size();
}

unsigned int Deck::getElement(unsigned int index)
{
    return cards[index];
}

unsigned int PlayerDeck::getNumberOfAces()
{
    unsigned int numberOfAces = 0;
    for(auto card: cards)
    {
        //card.isAces() OR card.slug='ace'
        if(i==11)
        {
            numberOfAces++;
        }
    }
    return numberOfAces;
}

unsigned int PlayerDeck::getGameValue()
{
    unsigned int numberOfAces = getNumberOfAces();
    unsigned int totalValue = calculateTotalValue();

    if(totalValue>21)
    {
        for(int i=0; i<numberOfAces; i++)
        {
            unsigned int potentialGameValue = totalValue-(10*(i+1));
            if(potentialGameValue<=21)
            {
                gameValue = potentialGameValue;
                return gameValue;
            }
        }
        //if it reaches here, it means ace being 11 or 1 doesn't matter; it's busted either way
        gameValue = 0;
        return gameValue;
    }
    else
    {
        gameValue = totalValue;
        return gameValue;
    }
}

void Deck::removeCard(unsigned int cardToRemove)
{
    bool error = true;

        for(int i=0; i<cards.size(); i++)
        {
            if(cards[i]==cardToRemove)
            {
                cards.erase(cards.begin()+i);
                error = false;
                break;
            }
        }
        if(error)
        {
            //either throw proper exception class and catch accordingly SOMEWHERE ELSE, or just apply the logic here without any throws
            throw "Card to remove couldn't be found!";
        }


}

unsigned int PlayerDeck::getOpenCardValue()
{
    try
    {
        if(getNumberOfCards()==0)
        {
            throw "There is no open card!";
        }
    }
    catch (const char* err)
    {
        std::cout<<err<<"\n";
        system("pause>0");
    }

    openCardValue = this->cards[0];//Another case for having a Card class. It would have an attribute of 'open' (bool). Also you need another method for getting open cards by this attribute filtering
    return openCardValue;
}

void Deck::addCard(unsigned int cardToAdd)
{
    //define card class, so you can easily assert & validate these card values. Otherwise you'll have to validate it in each method/func
    this->cards.push_back(cardToAdd);
}

void Deck::clearDeck()
{
    this->cards.clear();
}

void Deck::printCards()
{
    for(auto i: cards)
    {
        std::cout<<i<<" ";
    }
    std::cout<<"\n";
}

void GameDeck::createLargeDeck(int deckMultiplier)//define as static method which returns Deck/GameDeck instance
{
    clearDeck();

         //ordinary numbers
        for(int o=2; o<=10; o++)
        {
            for(int m=0; m<deckMultiplier; m++)
            {
                for(int i=1; i<=4; i++)//define a constant for this, it is the definition count for each card in a deck
                {
                    cards.push_back(o);
                }
            }

        }

        //k, q and j's
        for(int i=1; i<=12; i++)//no.
        {
            for(int m=0; m<deckMultiplier; m++)
            {
                cards.push_back(10);//Again, define card class so you can keep the labels, slugs and card values of these. Because as it is now, you don't know what the actual card is.
            }
        }

        //aces
        for(int i=0; i<4; i++)
        {
            for(int m=0; m<deckMultiplier; m++)
            {
                cards.push_back(11);
            }
        }

}

void Deck::equalizeDeck(Deck deckToCopy)
{
    this->cards = deckToCopy.cards;
}

void GameDeck::resetDeck(int deckMultiplier)
{
    clearDeck();
    createLargeDeck(deckMultiplier);
}

void GameDeck::removeCardsForTreeOperation(TreeNode node)
{
    unsigned int handSize = node.cardsInside.getNumberOfCards();
    unsigned int deckSize = getNumberOfCards();
    for(int i=handSize-1; i>0; i--)
    {
        unsigned int cardToRemove = node.cardsInside.getElement(i);
        removeCard(cardToRemove);
    }
}

void Player::printSubjectCards()
{
    cardsInsideHand.printCards();
}

unsigned Player::getPlayerOpenCardValue()
{
    openCardValue = this->cardsInsideHand.getOpenCardValue();
    return openCardValue;
}

unsigned Player::getPlayerGameValue()
{
    gameValue = cardsInsideHand.getGameValue();
    return gameValue;
}

std::vector<int> Player::getCards()
{
    return cardsInsideHand.cards;
}

unsigned Player::getNumberOfUnknownCards()
{
    try
    {
        if((cardsInsideHand.getNumberOfCards()-1)<0)
        {
            throw "Player can't have an unknown card";
        }
        else
        {
            return cardsInsideHand.getNumberOfCards()-1;
        }
    }
    catch (const char* err)
    {
        std::cout<<err<<"\n";
        system("pause>0");
    }

}

void Player::clearHand()
{
    cardsInsideHand.clearDeck();
}

void Player::drawSpecificCard(unsigned int cardToDraw, GameDeck &actualDeck)
{
    cardsInsideHand.addCard(cardToDraw);
    actualDeck.removeCard(cardToDraw);
}

void Glados::drawSpecificCard(unsigned int cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck)
{
    cardsInsideHand.addCard(cardToDraw);
    actualDeck.removeCard(cardToDraw);
    knownDeck.removeCard(cardToDraw);
}

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

TreeNode::TreeNode(PlayerDeck cards,unsigned int width,double parentProbability)
{
    this->cardsInside = cards;
    this->parentProbability = parentProbability;
    this->selfProbability = parentProbability * 1/(static_cast<double>(width));
}

void GameDeck::shuffle()
{
    try
    {
        if(cards.empty())
        {
            throw "Nothing to shuffle";
        }
        srand(time(0));
        unsigned int swapIndex;
        unsigned int swapCard;
        for(int i=0; i<333; i++)
        {
            swapIndex = rand()%cards.size();
            swapCard = cards[swapIndex];

            if(rand()%2==0)
            {
                cards.erase(cards.begin()+swapIndex);
                cards.push_back(swapCard);
            }
        }
    }
    catch (const char* err)
    {
        std::cout<<err<<"\n";
    }

}

GameDeck::GameDeck(unsigned int deckMultiplier)
{
    clearDeck();

   this->deckMultiplier = deckMultiplier;

    createLargeDeck(this->deckMultiplier);
}

ProbBar Glados::treeFunction(GameDeck originalDeck, unsigned int openCardValue)
{
    std::vector<TreeNode> nodeVector;
    PlayerDeck tempDeck;
    unsigned int deckSize = originalDeck.getNumberOfCards();
    for(int i=0; i<deckSize; i++)
    {
        tempDeck.addCard(openCardValue);
        tempDeck.addCard(originalDeck.cards[i]);
        TreeNode node(tempDeck,deckSize,1);
        nodeVector.push_back(node);
        tempDeck.clearDeck();
    }
    GameDeck tempImaginaryDeck(originalDeck.deckMultiplier);
    PlayerDeck tempNodeDeck;
    unsigned int lastRunFirstId = 0;
    unsigned int lastRunLastId = nodeVector.size();
    unsigned int nodeValue;
    bool treeOver;
    ProbBar probBar;
    while(true)
    {
        treeOver = true;
        for(unsigned int i=lastRunFirstId; i<lastRunLastId; i++)
        {
            tempImaginaryDeck = originalDeck;
            nodeValue = nodeVector[i].cardsInside.getGameValue();
            if(nodeValue<17 && nodeValue>0)
            {
                treeOver = false;
                //remove the cards that can't be drawn according to the current node
                tempImaginaryDeck.removeCardsForTreeOperation(nodeVector[i]);

                //now create the branches for this node
                unsigned int tempImaginaryDeckSize = tempImaginaryDeck.getNumberOfCards();

                for(int k=0; k<tempImaginaryDeckSize; k++)
                {
                    tempNodeDeck.equalizeDeck(nodeVector[i].cardsInside);
                    tempNodeDeck.addCard(tempImaginaryDeck.getElement(k));
                    TreeNode node(tempNodeDeck,tempImaginaryDeckSize,nodeVector[i].selfProbability);
                    nodeVector.push_back(node);
                }
            }
        }

        if(treeOver)
        {
            break;
        }
        lastRunFirstId = lastRunLastId;
        lastRunLastId = nodeVector.size();
    }
    for(int d=0; d<nodeVector.size(); d++)
    {
        if(nodeVector[d].gameValue==0 || nodeVector[d].gameValue>=17)
        {
            probBar.addProb(nodeVector[d].selfProbability,nodeVector[d].cardsInside.getGameValue());
        }

    }
    return probBar;
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

unsigned int Glados::getImaginaryHandValue(unsigned int imaginaryCardToDraw)
{
    cardsInsideHand.addCard(imaginaryCardToDraw);
    unsigned int imaginaryHandValue = getPlayerGameValue();
    cardsInsideHand.cards.pop_back();
    return imaginaryHandValue;
}

void Glados::updateExpectedValue(GameDeck originalDeck, unsigned int openCardValue)
{
    //TODO: DUPLICATE MAP
    unsigned int imaginaryCard;
    double expectedValue = 0;
    ProbBar probBar;
    GameDeck imaginaryDeck(originalDeck.deckMultiplier);
    imaginaryDeck.equalizeDeck(originalDeck);
    double initialWinProb;
    double imaginaryWinProb;
    probBar = treeFunction(originalDeck,openCardValue);
    initialWinProb = probBar.getWinProb(getPlayerGameValue());
    unsigned int imaginaryHandValue;
    probBar.clearBar();
    std::vector<unsigned int> duplicateVector;
    std::map<unsigned int, double> duplicateMap;
    for(int i=0; i<originalDeck.getNumberOfCards(); i++)
    {
        imaginaryCard = originalDeck.getElement(i);
        if(i!=0)
        {
            if(duplicateVector.back()==imaginaryCard)
            {
                expectedValue += duplicateMap[imaginaryCard];
                continue;
            }
        }
        imaginaryDeck.removeCard(imaginaryCard);
        imaginaryHandValue = getImaginaryHandValue(imaginaryCard);
        probBar = treeFunction(imaginaryDeck,openCardValue);
        imaginaryWinProb = probBar.getWinProb(imaginaryHandValue);
        expectedValue += (imaginaryWinProb-initialWinProb);
        imaginaryDeck.equalizeDeck(originalDeck);
        probBar.clearBar();
        duplicateVector.push_back(imaginaryCard);
        duplicateMap[imaginaryCard]=(imaginaryWinProb-initialWinProb);
    }
    this->expectedValueOfDrawingCard = expectedValue;
}

void ProbBar::clearBar()
{
    for(int i=0; i<6; i++)
    {
        probArray[i] = 0;
    }
}

void Player::addRoundScore()
{
    roundScore++;
}