//
// Created by yagiz on 2/22/2024.
//
#include "player.h"
#include "map"

void Player::drawImaginaryCard(card_t cardToDraw)
{
    cardsInsideHand.addCard(cardToDraw);
}

void Player::printSubjectCards()
{
    cardsInsideHand.printCards();
}

card_t Dealer::getPlayerOpenCard()
{
    return cardsInsideHand.getOpenCard();
}

handvalue_t Player::getPlayerGameValue()
{
    return cardsInsideHand.getGameValue();
}

void Player::clearHand()
{
    cardsInsideHand.clearDeck();
}

void Player::drawSpecificCard(card_t cardToDraw, GameDeck &actualDeck)
{
    if(actualDeck.getNumberOfCards()==0)
    {
        throw OutOfCards();
    }
    cardsInsideHand.addCard(cardToDraw);
    actualDeck.removeCard(cardToDraw);
}

void Player::addRoundScore()
{
    roundScore++;
}

void Player::drawRandomCard(GameDeck &actualDeck)
{
    if(actualDeck.getNumberOfCards()==0)
    {
        throw OutOfCards();
    }

    card_t randomCard = actualDeck.getRandomCard();
    cardsInsideHand.addCard(randomCard);
    actualDeck.removeCard(randomCard);

}

Glados::Glados(bool approximationMethod)
{
    this->approximation = approximationMethod;
}

void Glados::drawSpecificCard(card_t cardToDraw, GameDeck &actualDeck, GameDeck &knownDeck)
{
    if(actualDeck.getNumberOfCards()==0)
    {
        throw OutOfCards();
    }
    cardsInsideHand.addCard(cardToDraw);
    actualDeck.removeCard(cardToDraw);
    knownDeck.removeCard(cardToDraw);
}

ProbBar Glados::treeFunction(GameDeck originalDeck, card_t openCard)
{
    //something is fishy about this function. it only took like 2 hours. there is definitely a bug somewhere...

    std::vector<TreeNode> nodeVector;
    PlayerDeck tempDeck;
    unsigned int deckSize = originalDeck.getNumberOfCards();

    //the initial nodes
    for(int i=0; i<deckSize; i++)
    {
        tempDeck.addCard(openCard);
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
                    tempNodeDeck.copyDeck(nodeVector[i].cardsInside);
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


    for(auto & node : nodeVector)
    {
        handvalue_t gameValue = node.cardsInside.getGameValue();
        if(gameValue==0 || gameValue>=17)
        {
            probBar.addProb(node.selfProbability,gameValue);
        }
    }

    return probBar;
}

handvalue_t Glados::getImaginaryHandValue(card_t imaginaryCardToDraw)
{
    cardsInsideHand.addCard(imaginaryCardToDraw);
    handvalue_t imaginaryHandValue = getPlayerGameValue();
    cardsInsideHand.cards.pop_back();
    return imaginaryHandValue;
}

double Glados::expectedValueCaseDetector(double expectedValue,double initialWinProb,const unsigned int &iteration,const unsigned int &deckSize)
{
    double worstCaseScenario;
    double bestCaseScenario;

    worstCaseScenario = (deckSize-iteration)*(-initialWinProb);
    bestCaseScenario = (deckSize-iteration)*(1-initialWinProb);
    if(expectedValue+bestCaseScenario<0)
    {
        return -1.123;
    }
    else if(expectedValue+worstCaseScenario>0)
    {
        return 1.123;
    }
    else
    {
        return 0;
    }
}

handvalue_t Glados::getImaginaryHandValueCombinationHand(PlayerDeck playerDeckToAdd)
{
    for(unsigned int card: playerDeckToAdd.cards)
    {
        cardsInsideHand.addCard(card);
    }
    handvalue_t imaginaryHandValue = getPlayerGameValue();
    for(int i=0; i<playerDeckToAdd.getNumberOfCards(); i++)
    {
        cardsInsideHand.cards.pop_back();
    }
    return imaginaryHandValue;
}

void Glados::changeCalculationApproach()
{
    approximation = !approximation;
}

double Glados::getExpectedValue(GameDeck originalDeck, card_t openCard)
{

    if(getPlayerGameValue()==0)
    {
        return -1;
    }

    card_t imaginaryCard;
    double expectedValue = 0;
    ProbBar probBar;
    GameDeck imaginaryDeck(originalDeck.deckMultiplier);
    imaginaryDeck.copyDeck(originalDeck);
    double initialWinProb;
    double imaginaryWinProb;

    probBar = treeFunction(originalDeck,openCard);

    initialWinProb = probBar.getWinProb(getPlayerGameValue());

    std::cout<<"iwp "<<initialWinProb<<"\n";

    handvalue_t imaginaryHandValue;

    if(!approximation)
    {
        probBar.clearBar();
    }

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

        if(!approximation)
        {
            imaginaryDeck.removeCard(imaginaryCard);
        }

        imaginaryHandValue = getImaginaryHandValue(imaginaryCard);
        if(imaginaryHandValue==0)
        {
            imaginaryWinProb = 0;
        }
        else
        {
            if(!approximation)
            {
                probBar = treeFunction(imaginaryDeck,openCard);
            }

            imaginaryWinProb = probBar.getWinProb(imaginaryHandValue);
        }
        expectedValue += (imaginaryWinProb-initialWinProb);
        imaginaryDeck.copyDeck(originalDeck);

        if(!approximation)
        {
            probBar.clearBar();
        }

        duplicateVector.push_back(imaginaryCard);
        duplicateMap[imaginaryCard]=(imaginaryWinProb-initialWinProb);
    }

    std::cout<<"e: "<<expectedValue<<"\n";
    return expectedValue;


}

void Glados::drawRandomCard(GameDeck &actualDeck, GameDeck &knownDeck)
{
    if(actualDeck.getNumberOfCards()==0)
    {
        throw OutOfCards();
    }


    card_t randomCard = actualDeck.getRandomCard();
    cardsInsideHand.addCard(randomCard);

    actualDeck.removeCard(randomCard);
    knownDeck.copyDeck(actualDeck);
}


void Dealer::drawCardSoft17(GameDeck &actualDeck)
{
    handvalue_t gameValue = getPlayerGameValue();
    while(true)
    {
        if(actualDeck.getNumberOfCards()==0)
        {
            throw OutOfCards();
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
           throw std::logic_error("Out of cards!");
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
