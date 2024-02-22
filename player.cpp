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
                    continue;
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
        if(d.gameValue==0 || d.gameValue>=17)
        {
            probBar.addProb(d.selfProbability,d.cardsInside.getGameValue());
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

double Glados::getExpectedValue(GameDeck originalDeck, unsigned int openCardValue)
{

    //it's pretty damn hard to mathematically prove that if glados is busted, it doesn't matter if he chooses to draw a card or not
    //one could say "well, he might want to draw a card to increase the chance of dealer getting busted" but... no.
    //there are 3 scenarios: 1-there is no branch in the tree that ends up in dealer busting  2-there is no branch in tree that DOESN'T end up in dealer busting
    //and 3-there are branches that ends up in busting AND not. In the scenario 1, it's pretty obvious that no matter what Glados does, his probability of winning
    //doesn't change because in order for that to happen, dealer must also bust and that contradicts with the scenario itself.
    //In the scenario 2, drawing a card will end up not changing winning probability OR worsen it. Right now, our probability of winning is 1 (draw is also considered
    //a victory for a couple of reasons) and if we draw a card (let's say, like an 8) that might alter the tree in a way that now, there are some branches that doesn't
    //end up in dealer busting. And remember, Glados is busted so any kind of value is going to beat him.
    //And there is the damned scenario 3. Look, all you need to know is that the positive expected value of drawing low value cards (which alters the tree in a way
    //so that the probability of dealer busting increases than the initial condition) times the probability of drawing those cards card IS EQUAL to the negative
    //expected value of drawing high value cards (which alters the tree in way so that the probability of dealer busting increases than the initial condition)

    //of course, this is just a conjecture. need someone to prove or disprove it

    if(getPlayerGameValue()==0 || getPlayerGameValue()>19)
    {
        return -420.420;
    }

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

    double worstCaseScenario;
    double bestCaseScenario;

    for(int i=0; i<originalDeck.getNumberOfCards(); i++)
    {
        imaginaryCard = originalDeck.getElement(i);

        if(i!=0)
        {
            worstCaseScenario = (originalDeck.getNumberOfCards()-i)*(-initialWinProb);
            bestCaseScenario = (originalDeck.getNumberOfCards()-i)*(initialWinProb);
            if(expectedValue+bestCaseScenario<0)
            {
                std::cout<<"worst case scenario detected in iteration: "<<" "<<i<<"\n";
                expectedValue = -420.420;
                return expectedValue;
            }
            else if(expectedValue+worstCaseScenario>0)
            {
                std::cout<<"best case scenario detected in iteration: "<<" "<<i<<"\n";
                expectedValue = +420.420;
                return expectedValue;
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

unsigned int Glados::getLastCard()
{
    return cardsInsideHand.cards.back();
}

void Glados::drawCardBasedOnExpectedValue(GameDeck &actualDeck, GameDeck &knownDeck, const unsigned int &dealerOpenCardValue)
{
    double expectedValue;
    expectedValue = getExpectedValue(knownDeck,dealerOpenCardValue);
    while(expectedValue>0)
    {
        if(actualDeck.getNumberOfCards()<2)
        {
            actualDeck.createLargeDeck();
            knownDeck.createLargeDeck();
        }
        drawRandomCard(actualDeck,knownDeck);
        expectedValue = getExpectedValue(knownDeck,dealerOpenCardValue);
    }
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

void DealerCopycat::drawCardLikeDealer(Glados glados, GameDeck ghostDeck)
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