#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>

#include "subject.h"
#include "card.h"
#include "minion.h"
#include "ability.h"
#include "ritual.h"
#include "enchantment.h"

// Constants for beginning a game for a player
const int HP_START = 20;
const int MAGIC_START = 3;
const int FULL_HAND = 5;
const int FULL_BOARD = 5;
const int CARD_HORIZ_LEN = 33;

class PlayerBoard;
class Ability;
class Card;
class Ritual;
class Enchantment;

// This is the player class

class Player {

    std::string name;                               // Name of the Player
    int playerNum;                                  // Number of the Player
    int hp;                                         // Health points of Player
    int magic;                                      // Magic points of Player

    std::vector<Card*> deck;        // Deck of Player
    std::vector<Card*> hand;        // Hand of Player
    std::vector<Card*> board;       // Board of Player
    std::vector<Card*> graveyard;   // Graveyard of Player
    std::vector<Card*> ritual;      // Ritual of Player

    public:

    // Ctor of a Player with given name and player num
    Player(std::string name, int num);

    // Dtor
    virtual ~Player();
    // Determines the derived type of Card and frees it accordingly
    void pseudoDtor(Card * toDestroy);
    
    std::string getName();                  // Getter of name
    void setName(std::string newName);      // Setter of name

    int getHp();                            // Getter of hp
    void setHp(int num);                    // Setter of hp

    int getMagic();                         // Getter of magic
    void setMagic(int num);                 // Setter of magic

    void changeHp(int change);              // Changes hp by adding (subtracting) change
    void changeMagic(int change);           // Changes magic by adding (subtracting) change
    
    // Player's deck is made from card names in filename
    void makeDeck(std::string filename, bool isTesting);

    // newDeck is shuffled (if in testing mode)
    void shuffleDeck(std::vector<Card*> & newDeck);

    // Draws 5 cards for the player at the beginning of the game.
    void beginningDrawHand();

    // Takes the top card from the deck and moves it to the active player's hand
    void drawFromDeckToHand();

    // Discards a card from the hand
    void discardFromHand(int i, bool toDestroy);
    
    // Discards a card from Board (not sent to graveyard)
    void discardFromBoard(int i, bool toDestroy);

    // Takes the i-th card from the playboard and puts it in the graveyard
    void removeFromBoardToGraveYard(int i);

    // Takes the top card in the graveyard and puts it back onto the playboard, hand, or deck
    void drawTopFromGraveYard();

    // Ritual getter. Returns a typecasted Ritual pointer
    Ritual * getRitual();

    // Returns size of ritual vector. In a stable game state, this
    // will always be 1 
    int getRitualSize();

    // Returns true if player has a Ritual in play
    bool hasRitual();

    // Clears the ritual vector
    void removeRitual(bool toDestroy);

    // Returns size of Player's deck 
    int getDeckSize();

    // Returns size of Player's hand 
    int getHandSize();

    // Getter for number of cards on Player's board 
    int getBoardSize();

    // Getter for number of cards in Player's graveyard 
    int getGraveyardSize();
    
    // Adds newCard to Player's board
    void addCardToBoard(Card* newCard);

    // Player's minion at index myMinion attacks opponent's minion at index minionToAttack 
    void useMinionToAttack(Player * opponent, int myMinion, int minionToAttack);

    // Plays the i-th card from the active player's hand on the t-th card of player # p
    void playCardHasTarget(int i, Player * p, char t);

    // Plays the i-th card in the active player's hand
    void playCardNoTarget(int i, Player *opponent);
    
    // Plays a ritual card from hand
    void playRitualCard();

    // Returns the position of min in Player's board
    // Indexing begins at 0
    int findIndexInBoard(Minion * min);

    // Prints a minions information
    void printMinionInfo(int i);
    
    // Displays the hand of a player to output
    void displayPlayerHand();

    // Minion * getter for Cards on board 
    Minion * getElemFromBoard(int index);

    // Minion * getter for Cards in graveyard
    Minion * getMinionFromGraveyard(int index);

    // Card * getter for Cards in hand 
    Card * getElemFromHand(int index);

    // Enchantment * getter for Cards in hand 
    Enchantment * getEnchantmentFromHand(int index);
    
    // Ritual * getter for Cards in hand 
    Ritual * getRitualFromHand(int index);
    
    // Takes the minion on the board and puts it on hand
    void minionToHand(int i);

    // Damage all minions on Player's (this) board by damage
    void damageMinionsOnBoard(Player * activePlayer, Player * affectedPlayer, int damage);
    // Heal all minions on Player's (this) board by heal
    void healMinionsOnBoard(int heal);

    // Functions for display printing 
    std::string printBorderTopBot(bool top);
    std::string printBorderSide();
    std::vector<std::string> displayCardPlayer();
    std::vector<std::vector<std::string>> displayCardsBoard();
    std::vector<std::vector<std::string>> displayCardOuterRow (int playerNum);
    std::vector<std::string> displayGameTitle();

    // Restores action points of Player's minions in play
    void restoreMinionAction();

    // Observer methods
    void notifyMinions(Subject * whoNotified, State stateOfGame);
    void attachObserversToNewMinion(Card * newMinion);
    void attachNewMinionToObservers(Card * newMinion);

    friend class PlayerBoard;
};

#endif


