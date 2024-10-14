In this project, I have designed and implemented a bot that plays the card game "Blackjack" from stratch. Know that all the algorithms, formulas and the code for them are all designed by me. Not a single external library was used for this project.

In this readme file, I have focused on the operational methods and outcomes of formulas and algorithms rather than their reasons and logics. If you are asking questions like "Why?" or "How so?" you can examine the other document where I have explained in more detail.

Objective

Because I won’t dive into strategies that include split and bet raise (at least, not yet) the only decision left is to draw a card or not.
To generate a function that calculates whether it is more appropriate to draw a card or not, considering three parameters: the dealer's open card, the cards in our hand and other players' hands (which we can generalize as player cards excluding the dealer's cards on the table), and the known game deck.
If we formulate the question in a more specific format: Is the probability of winning when we don't draw a card less than or greater than the sum of the probabilities of winning that could occur when we do draw a card?
The formula we will use is as follows:

  ![image](https://github.com/Sokisati/blackjack_real/assets/129625653/09b231ba-f63b-46a8-8565-42a30fda4274)

Calculation of Winning Probability
The probability of winning is calculated using a tree generated with the known deck and the dealer's open card. In the tree created with the strategy of hitting below 17 and standing equal to or above, each leaf represents a potential hand the dealer could have. The probability of each node occurring is calculated as the probability of the parent node * ( 1/(the number of siblings it has +1) ).

![nt2-0](https://github.com/Sokisati/blackjack_real/assets/129625653/b2b8d6ea-9c05-47d4-a036-aef2cc36b6ab)


For example, in a scenario where the dealer's open card is 10 and the known deck consists of cards 2, 5, and 7, the tree that will be generated would be as described above. The ones indicated in blue represent the probability of the node occurring. It is essential to note that we consider only leaf nodes as potential hands, while other nodes are merely stops on the way to those potential hands.
Now that the tree has been generated, we need to create a map that represents the possible hands the dealer could have and their probabilities.
This map should have 6 keys and values: 0, 17, 18, 19, 20, 21. The keys represent the hand values, and the value represents the sum of the probabilities of those hand values. Returning to the tree mentioned earlier, the map value for that tree would be as follows:

![image](https://github.com/Sokisati/blackjack_real/assets/129625653/11e66681-ef08-464b-bdcf-83cc5a256e70)

Now, all we need to do is to place our hand value on this map and sum the probability values above it and to its left. (I have explained why the value above represents a winning situation despite being a tie in the other document.)

For example; if our hand value is 15, our probability of winning is 1/6, and if it's 18, it is 5/6.

Ultimate Test

We play hundreds of games against Glados and introduce a ghost player into this simulation. The cards drawn by this ghost player are not subtracted from the real deck. It mimics the dealer's strategy (hence I named it "copycat") and if possible, it copies Glados's cards.
For example:
Glados is dealt the first 2 cards, which are 10 and 5. The dealer's open card is 6. Glados calculates the expected value and decides not to draw as it turns out negative. However, the copycat, after obtaining the cards 10 and 5, wants to draw as it mimics the dealer's strategy.
In such situations, where Glados makes a different decision from the traditional dealer's strategy, we calculate what decision is superior in such situations using what I call the "copycat index."
The Copycat and the Index
If a situation (composed of 3 parameters: the known deck, the dealer's open card, and the open player cards) causes a discrepancy between Glados's strategy and the dealer's strategy, we need to calculate which decision is more profitable for that situation, or at least which one does not cause losses. For this, we send Glados, the dealer, and the copycat to parallel universes and repeat that situation a desired number of times (in my observations, usually 30 to 50 repetitions are sufficient; more than that takes an unnecessarily long time).

The reason I call it parallel universes is that the games they play until the number of repetitions is completed, similar to those in science fiction movies, do not affect the gain-loss scores held in the simulation; they only exist to increase or decrease the copycat index by 1.

![loop2](https://github.com/Sokisati/blackjack_real/assets/129625653/b04fbfdb-cb9e-4deb-a56c-2c67327b9305)

The loop works as follows: Firstly, Glados and the dealer (the copycat is currently not taking any action) play with each other for the specified number of repetitions. At the beginning of each "game," Glados is given the first 2 cards in the original game state, and the dealer is given the open card in the original state. (It is important to note that the deck they play with is the same as the original deck.) Whenever Glados wins against the dealer, the variable named "rope" (you will understand why it is named "rope" shortly) is incremented by +1, and whenever Glados loses, it is decremented by -1. After each game within this loop, the deck is reset to its initial state.
Once the desired number of repetitions is completed, the copycat starts to perform the same actions. However, when the copycat wins against the dealer, the "rope" variable is decremented by -1, and when it loses, it is incremented by +1.
As seen, similar to a tug-of-war competition, while Glados tries to pull the "rope" variable in the + direction, the copycat tries to pull it in the - direction.

![316849510-15c6baf7-2c52-48a0-9326-2b72db391cc7](https://github.com/Sokisati/blackjack_real/assets/129625653/4bc7415a-c0e3-481f-af9c-8a4e76d87088)


While not expected, it is acceptable for the rope to be less than 0 for a given disagreement situation. At the end of the every tug-of-war, copycat index is only incremented or subracted by 1.
What is not acceptable is for the copycat index to be less than 0 for a long simulation.

