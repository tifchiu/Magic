#ifndef PLAYBOARD_H
#define PLAYBOARD_H
#include <string>
#include <memory>
#include <vector>
#include "player.h"


// This is the PlayBoard class, it is the overall highest class and contains


// Height of cards
const int HEIGHT = 11;


class PlayBoard {

    Player * player1;           // Player 1 ptr
    Player * player2;           // Player 2 ptr
    bool isPlayer1Turn;         // True iff is it player 1's turn
    bool isTesting;             // Indication of testing mode
    
    public:

    // Ctor takes 2 player ptrs and if we are in testing mode
    PlayBoard(Player * player1, Player * player2, bool isTestMode);
    
    // Returns true if one of the players have won
    bool hasWon();

    // Prints out tie message
    void hasTie();

    // Prints out the board entirely
    void displayBoard();
    
    // Changes the turn on the end turn player's side
    void changeTurnEnd();

    // Changes the turn on the new turn player's side
    void changeTurnStart();

    // Dtor of the PlayBoard
    virtual ~PlayBoard() = default;

    // Returns true if it is player 1's turn
    bool getIsPlayer1Turn();

    // Returns true if the PlayBoard is in testing mode
    bool getIsTesting();

    // Get the ptr of the first player
    Player * getPlayer1();

    // Get the ptr of the 2nd player
    Player * getPlayer2();

    // Helper function to print out a row of cards on the board
    void rowPrint(std::vector<std::vector<std::string>> row);

    // Helper function to print out a card for rowPrint
    void printOutHelp();

    
    friend class Player;

};

#endif


