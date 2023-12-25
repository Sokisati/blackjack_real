#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

auto rng = std::default_random_engine {};

mt19937 mt(time(nullptr));

class Deck
{
private:
    vector<int> cards;
    int totalValue;
    int gameValue;
    int numberOfCards;
    int openCardValue;

public:

    void createDebugDeck()
    {
        cards.clear();
        cards.push_back(10);
        cards.push_back(6);
        cards.push_back(8);
        cards.push_back(2);
        cards.push_back(5);
        cards.push_back(4);
    }

    int getTotalValue()
    {
        int sum=0;
        for(int i=0; i<cards.size(); i++)
        {
            sum = sum + cards[i];
        }
        this->totalValue = sum;
        return totalValue;
    }

    int getNumberOfCards()
    {
        int number;
        number = cards.size();
        this->numberOfCards = number;
        return numberOfCards;
    }

    int getAceNumber()
    {
        int aceNumber = 0;
        for(int i=0; i<getNumberOfCards(); i++)
        {
            if(cards[i]==11)
            {
                aceNumber++;
            }
        }
        return aceNumber;
    }

    int getGameValue()
    {
        int value;
        int aceNumber;
        aceNumber = getAceNumber();
        if(getTotalValue()>21)
        {
            if(aceNumber==1)
            {
                value = getTotalValue()-10;
            }
            else if(aceNumber==2)
            {
                if(getTotalValue()-10<=21)
                {
                    value = getTotalValue()-10;
                }
                else
                {
                    value = getTotalValue()-20;
                }
            }
            else
            {
                value = 0;
            }
        }
        else
        {
            value = getTotalValue();
        }
        this->gameValue = value;
        return gameValue;
    }

    void removeCard(int cardToRemove)
    {
        for(int i=0; i<cards.size(); i++)
        {
            if(cards[i]==cardToRemove)
            {
                cards.erase(cards.begin()+i);
                break;
            }
        }
    }

    int getOpenCard()
    {
        int open;
        open = cards[0];
        openCardValue = open;
        return openCardValue;
    }

    void addCard(int cardToAdd)
    {
        this->cards.push_back(cardToAdd);
    }

    void clearCards()
    {
        this->cards.clear();
    }

    vector<int> getCards()
    {
        return cards;
    }

    void printCards()
    {
        for(int i=0; i<cards.size(); i++)
        {
            cout<<cards[i]<<" ";
        }
        cout<<endl;
    }

    int getElementI(int indexNumber)
    {
        return cards[indexNumber];
    }

    void createLargeDeck(int deckMultiplier)
    {
        this->cards.clear();
        //ordinary numbers

        for(int k=deckMultiplier; k>0; k--)
        {
            for(int k=2; k<=10; k++)
            {
                for(int i=1; i<=4; i++)
                {
                    cards.push_back(k);
                }
            }

            //k, q and j's
            for(int i=1; i<=12; i++)
            {
                cards.push_back(10);
            }

            //TODO: FIX ACE
            for(int i=0; i<4; i++)
            {
                cards.push_back(11);
            }
        }


    }

    void equalizeDeck(Deck deckToCopy)
    {
        cards = deckToCopy.getCards();
    }

    void resetDeck(int deckMultiplier)
    {
        clearCards();
        createLargeDeck(deckMultiplier);
    }

};

class Player{

private:
    int gameValue;
    int openCardValue;
    int numberOfCards;
    int totalValue;
    int roundScore = 0;
    int simulationScore = 0;
    Deck cardsInsideHand;

public:

    int getTotalNumberOfCards()
    {
        int number;
        number = cardsInsideHand.getNumberOfCards();
        this->numberOfCards = number;
        return numberOfCards;
    }

    int getTotalValueOfHand()
    {
        this->totalValue = cardsInsideHand.getTotalValue();
        return totalValue;
    }

    void printSubjectCards()
    {
        cardsInsideHand.printCards();
    }

    int getPlayerOpenCardValue()
    {
        this->openCardValue = cardsInsideHand.getOpenCard();
        return openCardValue;
    }

    int getPlayerGameValue()
    {
        gameValue = cardsInsideHand.getGameValue();
        return gameValue;
    }


    vector<int> getCards()
    {
        return cardsInsideHand.getCards();
    }

    void drawRandomCard(Deck &knownDeck, Deck &actualDeck)
    {
        int randomCard;
        int index = mt()%actualDeck.getNumberOfCards();
        randomCard = actualDeck.getElementI(index);
        this->cardsInsideHand.addCard(randomCard);

        knownDeck.removeCard(randomCard);
        actualDeck.removeCard(randomCard);
    }

    int getNumberOfUnknownCards()
    {
        return getTotalNumberOfCards()-1;
    }

    Deck getUnknownDeck()
    {
        Deck unKnownDeck;
        for(int i=1; i<cardsInsideHand.getNumberOfCards(); i++)
        {
            unKnownDeck.addCard(cardsInsideHand.getElementI(i));
        }
        return unKnownDeck;
    }

    void clearHand()
    {
        cardsInsideHand.clearCards();
    }

    void drawSpecificCard(int cardToAdd, bool isForGlados,Deck &knownDeck, Deck &actualDeck)
    {
        actualDeck.removeCard(cardToAdd);
        if(isForGlados)
        {
            cardsInsideHand.addCard(cardToAdd);
            knownDeck.removeCard(cardToAdd);
        }
        else
        {
            cardsInsideHand.addCard(cardToAdd);
        }

    }

    void addRoundScore()
    {
        roundScore++;
    }

    int getRoundScore()
    {
        return roundScore;
    }

    void resetRoundScore()
    {
        roundScore = 0;
    }

    void addSimulationScore()
    {
        simulationScore++;
    }

    int getSimulationScore()
    {
        return simulationScore;
    }

};

class handNode
{
public:

    int value = 0;
    int numberOfSiblings;
    double parentProbability;
    double selfProbability;
    bool finalHand;
    vector<int> cardVector;
    int openCardValue;

public:

    handNode(int nOSParam, double pPParam, vector<int> cardVectorParam)
    {
        numberOfSiblings = nOSParam;
        parentProbability = pPParam;
        selfProbability = static_cast<double>(1.0/numberOfSiblings)*parentProbability;
        cardVector = cardVectorParam;

        for(int i=0; i<cardVector.size(); i++)
        {
            value = value + cardVector[i];
        }

        if(value>=17)
        {
            finalHand = true;
        }
        else
        {
            finalHand = false;
        }
    }
};

vector<int> eraseFunction(int number, vector<int> vector)
{
    for(int i=0; i<vector.size(); i++)
    {
        if(vector[i]==number)
        {
            vector.erase(vector.begin()+i);
            break;
        }
    }
    return vector;
}

vector<handNode> dealerTreeInitialFunction(Deck knownDeck, int playerOpenCardValue, vector<handNode> handNodeVector)
{
    vector<int> tempVector;

    for(int i=0; i<knownDeck.getNumberOfCards(); i++)
    {
        tempVector.push_back(playerOpenCardValue);
        tempVector.push_back(knownDeck.getElementI(i));
        handNodeVector.emplace_back(knownDeck.getNumberOfCards(),1.0,tempVector);
        tempVector.clear();
    }

    return handNodeVector;
}

void dealerTreeRecursiveFunction(vector<handNode> &handNodeVector, Deck knownDeck, int lastRunLastNodeID, int lastRunFirstNodeID, int limit)
{
    vector<int> tempVector;
    vector<int> originalVector;
    vector<int> knownDeckCopy;
    knownDeckCopy = knownDeck.getCards();
    originalVector = knownDeckCopy;
    int nodesCreatedThisRun = 0;
    int firstNodeID;
    int lastNodeID;

    for(int i=lastRunFirstNodeID; i<=lastRunLastNodeID; i++)
    {
        if(handNodeVector[i].value<limit)
        {
            knownDeckCopy = originalVector;

            // Remove the cards that CAN'T be drawn, starting from the last element
            for (auto it = handNodeVector[i].cardVector.begin() + 1; it != handNodeVector[i].cardVector.end(); ++it)
            {
                int cardThatWillBeRemoved = *it; // Get the current card (excluding the open card value)
                knownDeckCopy = eraseFunction(cardThatWillBeRemoved,knownDeckCopy);
            }

            for(int create=0; create<knownDeckCopy.size(); create++)
            {
                tempVector = handNodeVector[i].cardVector;
                tempVector.push_back(knownDeckCopy[create]);
                handNodeVector.emplace_back(knownDeckCopy.size(),handNodeVector[i].selfProbability,tempVector);
                tempVector.clear();
                if(nodesCreatedThisRun==0 && create==0)
                {
                    firstNodeID = handNodeVector.size()-1;
                }
                nodesCreatedThisRun++;
            }
            knownDeckCopy = originalVector;
        }
    }
    knownDeckCopy = originalVector;
    lastNodeID = handNodeVector.size()-1;

    if(nodesCreatedThisRun!=0)
    {
        dealerTreeRecursiveFunction(handNodeVector,knownDeck,lastNodeID,firstNodeID,limit);
    }

}

vector<handNode> dealerTreeFunction(Deck knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVectorParameter, int satisfactionValue)
{
    vector<handNode> handNodeVector;
    vector<handNode> finalNodeVector;
    handNodeVector = dealerTreeInitialFunction(knownDeck,playerOpenCardValue,handNodeVectorParameter);
    dealerTreeRecursiveFunction(handNodeVector,knownDeck,knownDeck.getNumberOfCards()-1,0,satisfactionValue);

    for(int i=0; i<handNodeVector.size(); i++)
    {
        if(handNodeVector[i].finalHand)
        {
            finalNodeVector.push_back(handNodeVector[i]);
        }
    }

    return finalNodeVector;
}

vector<double> dealerTreeVectorFunction(Deck knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVectorParameter, int satisfactionValue)
{
    vector<handNode> finalNodes;
    finalNodes = dealerTreeFunction(knownDeck,playerOpenCardValue,handNodeVectorParameter,satisfactionValue);
    vector<double> probVector;
    double probArray[7];

    //for initial values
    for(int i=0; i<7; i++)
    {
        probArray[i]=0;
    }

    for(int i=0; i<finalNodes.size(); i++)
    {
        if(finalNodes[i].value>21)
        {
            probArray[0] += finalNodes[i].selfProbability;
        }
        else if(finalNodes[i].value==17)
        {
            probArray[1] += finalNodes[i].selfProbability;
        }
        else if(finalNodes[i].value==18)
        {
            probArray[2] += finalNodes[i].selfProbability;
        }
        else if(finalNodes[i].value==19)
        {
            probArray[3] += finalNodes[i].selfProbability;
        }
        else if(finalNodes[i].value==20)
        {
            probArray[4] += finalNodes[i].selfProbability;
        }
        else if(finalNodes[i].value==21)
        {
            probArray[5] += finalNodes[i].selfProbability;
        }
    }

    for(int i=0; i<6; i++)
    {
        probVector.push_back(probArray[i]);
    }

    //clear the cache
    handNodeVectorParameter.clear();

    return probVector;
}

int combinationFunction(int samplePool, int selection)
{
    int sum = 1;
    int selectionFactorial = 1;
    int result;

    if (samplePool == selection)
    {
        return 1;
    }

    for (int i = 0; i < selection; i++)
    {
        sum = sum * (samplePool - i);
    }

    for (int i = 0; i < selection - 1; i++)
    {
        selectionFactorial = selectionFactorial * (selection - i);
    }
    result = sum / selectionFactorial;
    return result;
}

void generateCombinations(Deck knownDeck, int cardDraw, int start_index, vector<int>& current_combination, vector<vector<int>>& combinations)
{
    if (current_combination.size() == cardDraw) {
        combinations.push_back(current_combination);
        return;
    }

    for (int i = start_index; i < knownDeck.getNumberOfCards(); ++i) {
        current_combination.push_back(knownDeck.getElementI(i));
        generateCombinations(knownDeck, cardDraw, i + 1, current_combination, combinations);
        current_combination.pop_back();
    }
}

vector<vector<int>> getCombinations(Deck knownDeck, int cardDraw) {
    vector<int> current_combination;
    vector<vector<int>> combinations;
    generateCombinations(knownDeck, cardDraw, 0, current_combination, combinations);
    return combinations;
}

double winProbabilityFunction(Deck knownDeck,int gladosHandValue,vector<double> dealerTreeVector)
{
    double winProb;
    if(gladosHandValue>21 || gladosHandValue<17)
    {
        winProb = dealerTreeVector[0];
    }
    else
    {
        if(gladosHandValue==17)
        {
            winProb = dealerTreeVector[0];
        }
        else if(gladosHandValue==18)
        {
            winProb = dealerTreeVector[0] + dealerTreeVector[1];
        }
        else if(gladosHandValue==19)
        {
            winProb =  dealerTreeVector[0] + dealerTreeVector[1] + dealerTreeVector[2];
        }
        else if(gladosHandValue==20)
        {
            winProb = dealerTreeVector[0] + dealerTreeVector[1] + dealerTreeVector[2] + dealerTreeVector[3];
        }
        else if(gladosHandValue==21)
        {
            winProb = dealerTreeVector[0] + dealerTreeVector[1] + dealerTreeVector[2] + dealerTreeVector[3] + dealerTreeVector[4];
        }
    }
    return winProb;
}

double expectedValueFunction(Deck knownDeck, int gladosHandValue, int dealerOpenCardValue,vector<handNode> &handNodeVector)
{

    double instanceSum;
    vector<double> probVector;
    probVector = dealerTreeVectorFunction(knownDeck,dealerOpenCardValue,handNodeVector,17);
    double initialWinProb = winProbabilityFunction(knownDeck,gladosHandValue,probVector);
    double sum = 0;
    int imaginaryHandValue;
    int imaginaryGameValue;
    double imaginaryWinProb;
    int theCard;
    int card1,card2;


    double sumTwo = 0;

    Deck imaginaryKnownDeck;
    imaginaryKnownDeck.equalizeDeck(knownDeck);
    probVector = dealerTreeVectorFunction(imaginaryKnownDeck,dealerOpenCardValue,handNodeVector,17);

    for(int i=0; i<knownDeck.getNumberOfCards(); i++)
    {
        theCard = imaginaryKnownDeck.getElementI(i);
        imaginaryHandValue = gladosHandValue + theCard;
        imaginaryWinProb = winProbabilityFunction(imaginaryKnownDeck,imaginaryHandValue,probVector);
        instanceSum = imaginaryWinProb - initialWinProb;
        sum += instanceSum;
    }

    handNodeVector.clear();


    //we should also account for 2 combinations. (I mean, I think...)
    int combinations = combinationFunction(imaginaryKnownDeck.getNumberOfCards(),2);
    vector<int> combinationVector;
    vector<vector<int>> allCombinations = getCombinations(knownDeck,2);
    for(int i=0; i<combinations; i++)
    {
        combinationVector = allCombinations[i];
        card1 = combinationVector.front();
        card2 = combinationVector.back();

        imaginaryHandValue = gladosHandValue + card1 + card2;
        if(card1==11 && card2==11)
        {
            imaginaryHandValue = imaginaryHandValue-20;
        }
        else if((card1==11 || card2==11)&&imaginaryHandValue>21)
        {
            imaginaryHandValue = imaginaryHandValue-10;
        }
        imaginaryWinProb = winProbabilityFunction(imaginaryKnownDeck,imaginaryHandValue,probVector);
        instanceSum = imaginaryWinProb - initialWinProb;
        sumTwo += instanceSum;
    }

    cout<<"two"<<sumTwo<<" one"<<sum<<endl;
    if(sumTwo>0 && sum<=0)
    {
        return 222;
    }


    handNodeVector.clear();

    double roundedValue = round(sum * 1e6) / 1e6;
    return roundedValue;
}


int main()
{

    mt19937 mt_rng(time(nullptr));
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    vector<handNode> handNodeVector;
    vector<handNode> debugTree;
    vector<double> dealerTree;


    int numberOfDecks = 1;
    int roundToWin = 6;
    double expectedValue;
    vector<int> simulationVector;
    int gladosCard1,gladosCard2,dealerOpenCard;

    Player Glados;
    Player Dealer;

    Deck actualDeck;
    Deck deckKnownToGlados;
    deckKnownToGlados.createLargeDeck(numberOfDecks);
    actualDeck.createLargeDeck(numberOfDecks);


    while(Glados.getRoundScore()<roundToWin && Dealer.getRoundScore()<roundToWin)
    {
        cout<<"Type which cards are dealt to me"<<endl;
        cin>>gladosCard1;
        cin>>gladosCard2;
        Glados.drawSpecificCard(gladosCard1,true,deckKnownToGlados,actualDeck);
        Glados.drawSpecificCard(gladosCard2,true,deckKnownToGlados,actualDeck);
        if(gladosCard1==404)
        {
            break;
        }

        cout<<"Type dealers open card"<<endl;
        cin>>dealerOpenCard;
        Dealer.drawSpecificCard(dealerOpenCard,true,deckKnownToGlados,actualDeck);
        deckKnownToGlados.removeCard(dealerOpenCard);
        dealerTree = dealerTreeVectorFunction(deckKnownToGlados,dealerOpenCard,handNodeVector,17);
        expectedValue = expectedValueFunction(deckKnownToGlados,Glados.getPlayerGameValue(),Dealer.getPlayerOpenCardValue(),handNodeVector);

        while(expectedValue>0&&Glados.getPlayerGameValue()>0)
        {
            cout<<"I want a card. Tell the dealer I want one"<<endl;
            cin>>gladosCard1;
            Glados.drawSpecificCard(gladosCard1,true,deckKnownToGlados,actualDeck);
            expectedValue = expectedValueFunction(deckKnownToGlados,Glados.getPlayerGameValue(),Dealer.getPlayerOpenCardValue(),handNodeVector);
            cout<<"glados "<<Glados.getPlayerGameValue()<<"\n";
        }
        cout<<"I don't want any/more cards. Now it's dealers turn"<<endl;
        cout<<"Type dealers secret cards"<<endl;

        while(Dealer.getPlayerGameValue()<17 && Dealer.getPlayerGameValue()>0)
        {
            cin>>dealerOpenCard;
            Dealer.drawSpecificCard(dealerOpenCard,false,deckKnownToGlados,actualDeck);
        }

        if(Glados.getPlayerGameValue()>Dealer.getPlayerGameValue())
        {
            cout<<"I won, as expected"<<endl;
            Glados.addRoundScore();
        }
        else if(Dealer.getPlayerGameValue()>Glados.getPlayerGameValue())
        {
            cout<<"You must have cheated or smth."<<endl;
            Dealer.addRoundScore();
        }
        else
        {
            cout<<"It's a draw. I hate draws"<<endl;
        }
        cout<<"\n";

        deckKnownToGlados.equalizeDeck(actualDeck);

        Glados.clearHand();
        Dealer.clearHand();
    }

    cout<<"glados score: "<<Glados.getRoundScore()<<endl;
    cout<<"dealer score: "<<Dealer.getRoundScore()<<endl;

    return 0;
}
