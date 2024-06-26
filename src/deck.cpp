//
// Created by yagiz on 2/22/2024.
//
#include "deck.h"



typedef unsigned int handvalue_t;

const char* AsyncDecks::what()
{
    return "Known deck async with actual deck";
}

const char* OutOfCards::what()
{
    return "Out of cards";
}

const char* CardRemovalError::what()
{
 return "Card to remove couldn't be found!";
}

unsigned int Deck::getNumberOfCards()
{
    return cards.size();
}

card_t Deck::getElement(unsigned int index)
{
    return cards[index];
}

void Deck::removeCard(card_t cardToRemove)
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
        throw CardRemovalError();
    }
}

void Deck::addCard(card_t cardToAdd)
{
    this->cards.push_back(cardToAdd);
}

void Deck::clearDeck()
{
    this->cards.clear();
}

void Deck::printCards()
{
    for(card_t card: cards)
    {
        std::cout<<card<<" ";
    }
    std::cout<<"\n";
}

void Deck::copyDeck(const Deck& deckToCopy)
{
    this->cards = deckToCopy.cards;
}

void Deck::writeCards(std::fstream &txt)
{
    for(card_t card : cards)
    {
        txt<<card<<" ";
    }
    txt<<"\n";
}

std::vector<std::vector<card_t>> Deck::getCardCombinations(unsigned int selectionSize)
{
    std::vector<std::vector<unsigned int>> combinations;
    std::vector<unsigned int> current_combination;
    generateCombinations(selectionSize, 0, current_combination, combinations);
    return combinations;
}

void Deck::generateCombinations(unsigned int selectionSize, unsigned int startIndex, std::vector<unsigned int>& currentCombination, std::vector<std::vector<unsigned int>>& combinations) {
    if (selectionSize == 0) {
        combinations.push_back(currentCombination);
        return;
    }

    for (unsigned int i = startIndex; i < cards.size(); ++i) {
        currentCombination.push_back(cards[i]);
        generateCombinations(selectionSize - 1, i + 1, currentCombination, combinations);
        currentCombination.pop_back();
    }
}


handvalue_t PlayerDeck::getTotalValue()
{
    unsigned int sum = 0;
    for(card_t card : cards)
    {
        sum += card;
    }
    return sum;
}

unsigned int PlayerDeck::getNumberOfAces()
{
    unsigned int numberOfAces = 0;
    for(card_t card : cards)
    {
        if(card==11)
        {
            numberOfAces++;
        }
    }
    return numberOfAces;
}

handvalue_t PlayerDeck::getGameValue()
{
    handvalue_t totalValue = getTotalValue();
    unsigned int numberOfAces = getNumberOfAces();

    if(totalValue>21)
    {
        for(int i=0; i<numberOfAces; i++)
        {
            handvalue_t potentialGameValue = totalValue-(10*(i+1));
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

card_t PlayerDeck::getOpenCard()
{
    if(getNumberOfCards()==0)
    {
        throw "There is no open card!";
    }

    return this->cards[0];
}


std::vector<PlayerDeck> GameDeck::createCombinationHands(std::vector<std::vector<card_t>> combinationVector)
{
    unsigned int selectionSize = combinationVector[0].size();
    std::vector<PlayerDeck> combinationHands;

    for(int i=0; i<combinationVector.size(); i++)
    {
        PlayerDeck tempDeck;
        for(int k=0; k<selectionSize; k++)
        {
            tempDeck.addCard(combinationVector[i][k]);
        }
        combinationHands.push_back(tempDeck);
    }

    return combinationHands;
}

GameDeck::GameDeck(unsigned int deckMultiplier)
{
    clearDeck();

    this->deckMultiplier = deckMultiplier;

    rng.seed(std::random_device()());
    createLargeDeck();
}

card_t GameDeck::getRandomCard()
{
    std::uniform_int_distribution<unsigned int> distribution(0, cards.size() - 1);
    return cards[distribution(rng)];
}

void GameDeck::createDebugDeck()
{
    cards.clear();
    addCard(2);
    addCard(3);
    addCard(5);
    addCard(5);
    addCard(10);


}

void GameDeck::createLargeDeck()
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

void GameDeck::resetDeck()
{
    clearDeck();
    createLargeDeck();
}

void GameDeck::removeCardsForTreeOperation(PlayerDeck nodeCards)
{
    for(unsigned int i=nodeCards.getNumberOfCards()-1; i>0; i--)
    {
        unsigned int cardToRemove = nodeCards.getElement(i);
        removeCard(cardToRemove);
    }
}
