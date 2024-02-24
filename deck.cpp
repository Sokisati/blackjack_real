//
// Created by yagiz on 2/22/2024.
//
#include "deck.h"
#include "iostream"

unsigned int Deck::getNumberOfCards()
{
    return cards.size();
}

unsigned int Deck::getElement(unsigned int index)
{
    return cards[index];
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
    //TODO
    if(error)
    {
            throw "Card to remove couldn't be found!";
    }
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

void Deck::equalizeDeck(const Deck& deckToCopy)
{
    this->cards = deckToCopy.cards;
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

unsigned int PlayerDeck::getOpenCardValue()
{
    if(getNumberOfCards()==0)
    {
        throw "There is no open card!";
    }

    return this->cards[0];
}


GameDeck::GameDeck(unsigned int deckMultiplier)
{
    clearDeck();

    this->deckMultiplier = deckMultiplier;

    createLargeDeck();
}

void GameDeck::createDebugDeck()
{
    cards.clear();
    cards.push_back(4);
    cards.push_back(4);
    cards.push_back(5);
    cards.push_back(5);
    cards.push_back(7);
    cards.push_back(7);
    cards.push_back(7);
    cards.push_back(8);
    cards.push_back(11);
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
    for(int i=nodeCards.getNumberOfCards()-1; i>0; i--)
    {
        unsigned int cardToRemove = nodeCards.getElement(i);
        removeCard(cardToRemove);
    }
}