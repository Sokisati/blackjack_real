//
// Created by yagiz on 1/29/2024.
//
#include "classes.h"

void GameDeck::createDebugDeck()
{
    cards.clear();
    cards.push_back(10);
    cards.push_back(10);
    cards.push_back(10);
    cards.push_back(6);
    cards.push_back(8);
    cards.push_back(2);
    cards.push_back(2);
}

unsigned int PlayerDeck::getTotalValue()
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
    this->numberOfCards = cards.size();
    return numberOfCards;
}

unsigned int Deck::getElement(unsigned int index)
{
    return cards[index];
}

unsigned int PlayerDeck::getNumberOfAces()
{
    unsigned int numberOfAces = 0;
    for(auto i: cards)
    {
        if(i==11)
        {
            numberOfAces++;
        }
    }
    return numberOfAces;
}

unsigned int PlayerDeck::getGameValue()
{
    unsigned int totalValue = getTotalValue();
    unsigned int numberOfAces = getNumberOfAces();

    if(totalValue>21)
    {
        for(int i=0; i<numberOfAces; i++)
        {
            unsigned int potentialGameValue = totalValue-(10*(i+1));
            if(potentialGameValue<=21)
            {
                return potentialGameValue;
            }
        }
        //if it reaches here, it means ace being 11 or 1 doesn't matter; it's busted either way
        return 0;
    }
    else
    {
        return totalValue;
    }
}

void Deck::removeCard(unsigned int cardToRemove)
{
    bool error = true;
    try
    {
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
            throw "Card to remove couldn't be found!";
        }
    }
    catch(const char* err)
    {
        std::cout<<err<<"\n";
        system("pause>0");
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

    return this->cards[0];
}

void Deck::addCard(unsigned int cardToAdd)
{
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

void GameDeck::createLargeDeck(int deckMultiplier)
{
    clearDeck();
         //ordinary numbers
        for(int o=2; o<=10; o++)
        {
            for(int m=0; m<deckMultiplier; m++)
            {
                for(int i=1; i<=4; i++)
                {
                    cards.push_back(o);
                }
            }

        }

        //k, q and j's
        for(int i=1; i<=12; i++)
        {
            for(int m=0; m<deckMultiplier; m++)
            {
                cards.push_back(10);
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
    drawGhostCard(glados.cardsInsideHand.getElement(cardsInsideHand.getNumberOfCards()));
}

unsigned int Glados::getLastCard()
{
    return cardsInsideHand.cards.back();
}

void Player::drawImaginaryCard(unsigned int cardToDraw)
{
    cardsInsideHand.addCard(cardToDraw);
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
                tempImaginaryDeck.removeCardsForTreeOperation(nodeVector[i]);

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

Table::Table()
{
    simResults.open("sim_results.txt",std::ios::out);
}

void Glados::updateExpectedValue(GameDeck originalDeck, unsigned int openCardValue)
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

    if(getPlayerGameValue()==0)
    {
        this->expectedValueOfDrawingCard = -420.420;
        return;
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
                this->expectedValueOfDrawingCard = -420.420;
                return;
            }
            else if(expectedValue+worstCaseScenario>0)
            {
                this->expectedValueOfDrawingCard = +420.420;
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
    this->expectedValueOfDrawingCard = expectedValue;
}

void ProbBar::clearBar()
{
    for(double & i : probArray)
    {
        i = 0;
    }
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

void DealerCopycat::drawGhostCard(unsigned int cardToAdd)
{
    cardsInsideHand.addCard(cardToAdd);
}

void Table::startNormalGame()
{
    unsigned int firstInitialCard;
    unsigned int secondInitialCard;
    unsigned int dealerOpenCard;
    unsigned int gladosExtraCard;
    unsigned int dealerSecretCard;
    unsigned int copycatExtraCard;


    GameDeck actualDeck(1);
    GameDeck knownDeck(1);

    actualDeck.createLargeDeck(1);
    knownDeck.createLargeDeck(1);


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
        glados.updateExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());
        while(glados.expectedValueOfDrawingCard>0)
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
        knownDeck.printCards();
        actualDeck.printCards();

    }

    std::cout<<"glados: "<<glados.roundScore<<" "<<" dealer:"<<dealer.roundScore<<"\n";

}

void Player::writeSubjectCardsTxt(std::fstream &fileToWrite)
{
    for(unsigned int i: cardsInsideHand.cards)
    {
        fileToWrite<<i<<" ";
    }

    std::cout<<"\n";
}

void Table::startSimulation(unsigned int roundToWin, unsigned int simulationToWin)
{

    GameDeck actualDeck(1);
    GameDeck knownDeck(1);

    GameDeck ghostDeck(1);

    actualDeck.createLargeDeck(1);
    knownDeck.createLargeDeck(1);

    int cc = 0;
    bool disregard;

    while(glados.simulationScore<simulationToWin && dealer.simulationScore<simulationToWin)
    {
        disregard = false;
        if(actualDeck.getNumberOfCards()<5)
        {
            actualDeck.createLargeDeck(1);
            knownDeck.createLargeDeck(1);
        }
        glados.drawRandomCard(actualDeck,knownDeck);
        glados.drawRandomCard(actualDeck,knownDeck);
        dealer.drawRandomCard(actualDeck);
        dealer.drawRandomCard(actualDeck);

        copycat.cardsInsideHand.equalizeDeck(glados.cardsInsideHand);

        ghostDeck.equalizeDeck(actualDeck);

        knownDeck.removeCard(dealer.getPlayerOpenCardValue());
        glados.updateExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());

        while(glados.expectedValueOfDrawingCard>0)
        {
            if(actualDeck.getNumberOfCards()<2)
            {
                actualDeck.createLargeDeck(1);
                knownDeck.createLargeDeck(1);
            }
            glados.drawRandomCard(actualDeck,knownDeck);
            glados.updateExpectedValue(knownDeck,dealer.getPlayerOpenCardValue());
        }


        while(true)
        {
            if(ghostDeck.getNumberOfCards()<1)
            {
                disregard = true;
                break;
            }
            if(copycat.getPlayerGameValue()>=17 || copycat.getPlayerGameValue()==0)
            {
                break;
            }
            bool canMirror = copycat.canMirrorCard(glados);

            if(canMirror)
            {
                copycat.mirrorCard(glados);
            }
            else
            {
                copycat.drawRandomCard(ghostDeck);
            }
        }

        while(true)
        {
            if(actualDeck.getNumberOfCards()<2)
            {
                actualDeck.createLargeDeck(1);
                knownDeck.createLargeDeck(1);
            }
            if(dealer.getPlayerGameValue()>=17 || dealer.getPlayerGameValue()==0)
            {
                break;
            }
            dealer.drawRandomCard(actualDeck);
        }

        std::cout<<"Glados hand: ";
        glados.printSubjectCards();
        std::cout<<"Dealer hand: ";
        dealer.printSubjectCards();
        std::cout<<"Copycat hand:";
        copycat.printSubjectCards();

        //TODO: TEST
        glados.writeSubjectCardsTxt(simResults);
        simResults<<"wow"<<"\n";

        if(glados.getPlayerGameValue()>dealer.getPlayerGameValue())
        {
            if(!disregard&&(copycat.getPlayerGameValue()<dealer.getPlayerGameValue() || copycat.getPlayerGameValue()==dealer.getPlayerGameValue()))
            {
                cc++;
            }
            std::cout<<"I won, as expected."<<"\n";
            glados.addRoundScore();
        }
        else if(glados.getPlayerGameValue()<dealer.getPlayerGameValue())
        {
            if(!disregard&&(copycat.getPlayerGameValue()>dealer.getPlayerGameValue()||copycat.getPlayerGameValue()==dealer.getPlayerGameValue()))
            {
                cc--;
            }
            std::cout<<"You must have cheated or smth."<<"\n";
            dealer.addRoundScore();
        }
        else
        {
            if(!disregard&&(copycat.getPlayerGameValue()<dealer.getPlayerGameValue()))
            {
                cc++;
            }
            else if(!disregard&&(copycat.getPlayerGameValue()>dealer.getPlayerGameValue()))
            {
                cc--;
            }
            std::cout<<"It's a draw. I hate draws."<<"\n";
        }

        std::cout<<"\n";

        glados.clearHand();
        dealer.clearHand();
        copycat.clearHand();
        knownDeck.equalizeDeck(actualDeck);
        ghostDeck.equalizeDeck(actualDeck);

        std::cout<<"gr: "<<glados.roundScore<<" dr:"<<dealer.roundScore<<"\n";
        std::cout<<"gs: "<<glados.simulationScore<<" ds: "<<dealer.simulationScore<<"\n";
        std::cout<<"cc: "<<cc<<"\n";

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
    }
    std::cout<<"glados: "<<glados.simulationScore<<" "<<" dealer:"<<dealer.simulationScore<<"\n";
    std::cout<<"cc: "<<cc<<"\n";
}