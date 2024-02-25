//
// Created by yagiz on 2/22/2024.
//
#include "player.h"
#include <random>
#include "map"
#include "iostream"

void Player::drawImaginaryCard(unsigned int cardToDraw)
{
    cardsInsideHand.addCard(cardToDraw);
}

void Player::printSubjectCards()
{
    cardsInsideHand.printCards();
}

unsigned Player::getPlayerOpenCardValue()
{
    return cardsInsideHand.getOpenCardValue();;
}

unsigned Player::getPlayerGameValue()
{
    return cardsInsideHand.getGameValue();
}

std::vector<int> Player::getCards()
{
    return cardsInsideHand.cards;
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

void Player::addRoundScore()
{
    roundScore++;
}

void Player::drawRandomCard(GameDeck &actualDeck) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, actualDeck.getNumberOfCards() - 1);

    unsigned int randomIndex = dis(gen);
    unsigned int randomCard = actualDeck.getElement(randomIndex);

    actualDeck.removeCard(randomCard);
    cardsInsideHand.addCard(randomCard);
}


void Glados::drawSpecificCard(unsigned int cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck)
{
    cardsInsideHand.addCard(cardToDraw);
    actualDeck.removeCard(cardToDraw);
    knownDeck.removeCard(cardToDraw);
}

ProbBar Glados::treeFunction(GameDeck originalDeck, unsigned int openCardValue)
{
    //something is fishy about this function. it only took like 2 hours. there is definitely a bug somewhere...

    std::vector<TreeNode> nodeVector;
    PlayerDeck tempDeck;
    unsigned int deckSize = originalDeck.getNumberOfCards();

    //the initial nodes
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
                tempImaginaryDeck.removeCardsForTreeOperation(nodeVector[i].cardsInside);

                //now create the branches for this node
                unsigned int tempImaginaryDeckSize = tempImaginaryDeck.getNumberOfCards();


                if(tempImaginaryDeckSize==0)
                {
                    throw TreeGenerationError();
                }


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


    for(auto & d : nodeVector)
    {

        unsigned int gameValue = d.cardsInside.getGameValue();
        if(gameValue==0 || gameValue>=17)
        {
            probBar.addProb(d.selfProbability,gameValue);
        }
    }

    return probBar;
}

unsigned int Glados::getImaginaryHandValue(unsigned int imaginaryCardToDraw)
{
    cardsInsideHand.addCard(imaginaryCardToDraw);
    unsigned int imaginaryHandValue = getPlayerGameValue();
    cardsInsideHand.cards.pop_back();
    return imaginaryHandValue;
}

double Glados::expectedValueCaseDetector(double expectedValue,double initialWinProb,const unsigned int &iteration,const unsigned int &deckSize)
{
    double worstCaseScenario;
    double bestCaseScenario;

    worstCaseScenario = (deckSize-iteration)*(-initialWinProb);
    bestCaseScenario = (deckSize-iteration)*(initialWinProb);
    if(expectedValue+bestCaseScenario<0)
    {
        std::cout<<"worst case scenario detected in iteration: "<<" "<<iteration<<"\n";
        return -1.123;
    }
    else if(expectedValue+worstCaseScenario>0)
    {
        std::cout<<"best case scenario detected in iteration: "<<" "<<iteration<<"\n";
        return 1.123;
    }
    else
    {
        return 0;
    }
}

double Glados::getExpectedValue(GameDeck originalDeck, unsigned int openCardValue)
{

    unsigned int imaginaryCard;
    double expectedValue = 0;
    ProbBar probBar;
    GameDeck imaginaryDeck(originalDeck.deckMultiplier);
    imaginaryDeck.equalizeDeck(originalDeck);
    double initialWinProb;
    double imaginaryWinProb;
    probBar = treeFunction(originalDeck,openCardValue);

    initialWinProb = probBar.getWinProb(getPlayerGameValue());

    std::cout<<"iwp "<<initialWinProb<<"\n";

    unsigned int imaginaryHandValue;
    probBar.clearBar();
    std::vector<unsigned int> duplicateVector;
    std::map<unsigned int, double> duplicateMap;

    double caseScenario;


    for(int i=0; i<originalDeck.getNumberOfCards(); i++)
    {

        imaginaryCard = originalDeck.getElement(i);
        if(i!=0)
        {
            caseScenario = expectedValueCaseDetector(expectedValue,initialWinProb,i,originalDeck.getNumberOfCards());
            if(caseScenario!=0)
            {
                return caseScenario;
            }
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

    std::cout<<"e: "<<expectedValue<<"\n";
    return expectedValue;
}

void Glados::drawRandomCard(GameDeck &actualDeck, GameDeck &knownDeck) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, actualDeck.getNumberOfCards() - 1);

    unsigned int randomIndex = dis(gen);
    unsigned int randomCard = actualDeck.getElement(randomIndex);

    actualDeck.removeCard(randomCard);
    knownDeck.removeCard(randomCard);
    cardsInsideHand.addCard(randomCard);
}

void Glados::drawCardBasedOnExpectedValue(GameDeck &actualDeck, GameDeck &knownDeck, const unsigned int &dealerOpenCardValue)
{

    double expectedValue;
    unsigned int gameValue;
    expectedValue = getExpectedValue(knownDeck,dealerOpenCardValue);
    while(expectedValue>0)
    {
       gameValue = getPlayerGameValue();
        if(gameValue==0)
        {
            return;
        }
        if(actualDeck.getNumberOfCards()<2)
        {
            actualDeck.createLargeDeck();
            knownDeck.createLargeDeck();
        }
        drawRandomCard(actualDeck,knownDeck);
        expectedValue = getExpectedValue(knownDeck,dealerOpenCardValue);
    }
}

void Glados::updateValueGained(GameDeck finalKnownDeck, PlayerDeck copycatHand, unsigned int openCardValue)
{
    unsigned int gladosHandSize = cardsInsideHand.getNumberOfCards();
    unsigned int copycatHandSize = copycatHand.getNumberOfCards();
    if(gladosHandSize==copycatHandSize)
    {
        return;
    }

    ProbBar probBar;
    double gladosStrategyWinProb;
    double copycatStrategyWinProb;

    probBar = treeFunction(finalKnownDeck,openCardValue);
    gladosStrategyWinProb = probBar.getWinProb(cardsInsideHand.getGameValue());

    if(gladosHandSize>copycatHandSize)
    {
        for(unsigned int i=gladosHandSize; i>copycatHandSize; i--)
        {
            finalKnownDeck.addCard(cardsInsideHand.getElement(i-1));
        }
         probBar = treeFunction(finalKnownDeck,openCardValue);
         copycatStrategyWinProb = probBar.getWinProb(copycatHand.getGameValue());
    }
    else
    {
        for(unsigned int i=copycatHandSize; i>gladosHandSize; i--)
        {
            finalKnownDeck.removeCard(copycatHand.getElement(i-1));
        }
        probBar = treeFunction(finalKnownDeck,openCardValue);
        copycatStrategyWinProb = probBar.getWinProb(copycatHand.getGameValue());
    }
    std::cout<<(gladosStrategyWinProb-copycatStrategyWinProb)<<"\n";
    valueGained += (gladosStrategyWinProb-copycatStrategyWinProb);
}



void Dealer::drawCardSoft17(GameDeck &actualDeck, GameDeck &knownDeck)
{
    unsigned int gameValue = getPlayerGameValue();
    while(true)
    {
        if(actualDeck.getNumberOfCards()<2)
        {
            actualDeck.createLargeDeck();
            knownDeck.createLargeDeck();
        }
        if(gameValue>=17 || gameValue==0)
        {
            break;
        }
        drawRandomCard(actualDeck);
        gameValue = getPlayerGameValue();
    }
}


bool DealerCopycat::canMirrorCard(Glados glados)
{
    if(glados.cardsInsideHand.getNumberOfCards()>cardsInsideHand.getNumberOfCards())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DealerCopycat::mirrorCard(Glados glados)
{
    drawImaginaryCard(glados.cardsInsideHand.getElement(cardsInsideHand.getNumberOfCards()));
}

void DealerCopycat::drawCardLikeDealer(const Glados& glados, GameDeck ghostDeck)
{
    while(true)
    {
        if(ghostDeck.getNumberOfCards()<1)
        {
            break;
        }
        if(getPlayerGameValue()>=17 || getPlayerGameValue()==0)
        {
            break;
        }
        bool canMirror = canMirrorCard(glados);

        if(canMirror)
        {
            mirrorCard(glados);
        }
        else
        {
            drawRandomCard(ghostDeck);
        }
    }
}
