#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <cctype>

#include "sorceryException.h"
#include "cardconstants.h"
#include "carddictionary.h"
#include "playboard.h"
#include "player.h"

// // // // // // // // // // // // // // // // // // // // // // // // // // //
// Sorcery - CS246 1209 Final Project

// Created by: Mark Chen (m375chen), Racheal Ou (r6ou), Tiffany Chiu (t3chiu)

// This is a text based C++ OOP designed game called Sorcery. It is a 2 player 
// game that mimics card games like Yu-Gi-Oh, Pokemon, or Hearthstone.

// // // // // // // // // // // // // // // // // // // // // // // // // // //

// Consts for command line args
const std::string DECK1_FLAG = "-deck1";
const std::string DECK2_FLAG = "-deck2";
const std::string INIT_FLAG = "-init";
const std::string TESTING_MODE_FLAG = "-testing";
const std::string DEFAULT_DECK = "default.deck";
const int NO_PARAM = -1;

// argPositions(argc, argv) takes in the args from main directly.
// Returns command line arguments in an ordered array
int * argPositions(int argc, char **argv) {
    // Init
    int index = 1;
    int deck1Position = NO_PARAM;
    int deck2Position = NO_PARAM;
    int initFilePosition = NO_PARAM;
    int testingMode = NO_PARAM;
    
    // We take arguments until all of them have been considered
    while (index < argc) {
        try {
            // Deck 1 indicator
            if (argv[index] == DECK1_FLAG) {
                
                // Index exceeds number of args
                if (index+1 >= argc) {
                    throw SorceryException("Deck 1 File flag used, but no file was provided.");
                } else {
                    // Set deck 1
                    deck1Position = index + 1;
                }

                // Continue in args
                index += 2;
            
            // Deck 2 indicator
            } else if (argv[index] == DECK2_FLAG) {
                if (index+1 >= argc) {
                    throw SorceryException("Deck 2 File flag used, but no file was provided.");
                } else {
                    deck2Position = index + 1;
                }
                index += 2;
            
            // Init file indicator
            } else if (argv[index] == INIT_FLAG) {
                if (index+1 >= argc) {
                    throw SorceryException("Init File flag used, but no file was provided.");
                } else {
                    initFilePosition = index + 1;
                }
                index += 2;
            
            // Testing mode indicator
            } else if (argv[index] == TESTING_MODE_FLAG) {
                testingMode = index;
                index += 1;
            
            // None of the above flags were used, but args exist
            } else {
                throw SorceryException("Unknown flag/argument given. 'quit' to restart if desired.");
            } 

        } catch (SorceryException & e) {
            // Move to next arg as current index is not right
            index++;
            // Output game system issue
            std::cerr << e.message << std::endl;
        }
    }
    // Create an array to give to game creation process
    static int positions[4] = { deck1Position, deck2Position, initFilePosition, testingMode };
    return positions;
}

// isNumber(str) Checks if a string (str) is a non-negative integer
// Returns a respective boolean
bool isNumber(std::string str){
    int len = str.length();
    
    // For every character in the potential number
    for (int i = 0; i < len; i++) {
        // As soon as one character is not a digit, the string is not a number
        if (std::isdigit(str[i]) == false) {
            return false;
        }
    }
    return true;
}

// splitCommand(cmd, command) turns a string command (cmd) into a command components (command)
void splitCommand(std::string cmd, std::vector<std::string> & command){
    std::string word = "";

    // Every command is split by white spacing
    for (auto letter : cmd) {
        // Add the component if we see white space
        if ((letter == ' ') && (word != "")) {
            command.push_back(word);
            // Reset the next word
            word = "";
        } else {
            word = word + letter;
        }
    }
    command.push_back(word);
}

// runGameLoop(game, command) applies the effect of a given command on the game
// Returns a boolean indication of when a game finishes
bool runGameLoop(PlayBoard & game, std::vector<std::string> & command) {
    std::string firstCommand = command[0];
    bool endOfGame = false;

    try {

        // Help command
        if ( firstCommand == "help" ) {

            game.printOutHelp();

        // End turn command
        } else if ( firstCommand == "end" ) {

            game.changeTurnEnd();
            game.changeTurnStart();

        // Quit command
        } else if ( firstCommand == "quit" ) {

            game.hasTie();
            endOfGame = true;

        // Draw card command
        } else if ( firstCommand == "draw" ) {
                
            // Only available in testing mode
            if (game.getIsTesting()) {
                // Draw card based on which player's turn it is
                if (game.getIsPlayer1Turn()) {
                    game.getPlayer1()->drawFromDeckToHand();
                } else {
                    game.getPlayer2()->drawFromDeckToHand();
                } // if
            } else {
                throw SorceryException("Draw command is only enabled with the -testing flag.");
            } // if
            
        // Discard command
        } else if ( firstCommand == "discard" ) {
            if (!game.getIsTesting()) {
                throw SorceryException("Discard is only enabled with the -testing flag.");
            }
            // Only discard if the game is in testing mode, and command arg is valid
            if ((command.size() >= 2) && isNumber(command[1])) {
                
                // Change the string to an integer
                int num = std::stoi(command[1]);

                // Discard and destroy the card at num-index for the active player
                if (game.getIsPlayer1Turn()) {
                    game.getPlayer1()->discardFromHand(num, true);
                } else {
                    game.getPlayer2()->discardFromHand(num, true);
                } // if
            } // if

        // Attack command
        } else if ( firstCommand == "attack") {

            // Given command attack i j, the i-th minion attacks the j-th opponent's minion
            if ((command.size() >= 3) && isNumber(command[1]) && isNumber(command[2])) {
                int i = stoi(command[1]);
                int j = stoi(command[2]);

                if (game.getIsPlayer1Turn()) {
                    // Player 1's i-th minion attacks the j-th minion of player 2
                    game.getPlayer1()->useMinionToAttack(game.getPlayer2(), i, j);
                } else {
                    // Player 2's i-th minion attacks the j-th minion of player 1
                    game.getPlayer2()->useMinionToAttack(game.getPlayer1(), i, j);
                }
            
            // Given command attack i, the i-th minion attack the opposing player
            } else if ((command.size() >= 2) && isNumber(command[1])) {
                int i = stoi(command[1]);

                if (game.getIsPlayer1Turn()) {
                    // Player 1's i-th minion attacks player 2
                    game.getPlayer1()->getElemFromBoard(i)->attackPlayer(game.getPlayer2());
                } else {
                    // Player 2's i-th minion attacks player 1
                    game.getPlayer2()->getElemFromBoard(i)->attackPlayer(game.getPlayer1());
                } // if
            } // if

            // Check if a human is dead yet
            endOfGame = game.hasWon();
                
        } else if ( firstCommand == "play") {

            // Given command play i p t,
            // the i-th card in hand is played on the t-th minion or ritual of player p
            if ((command.size() == 4) && isNumber(command[1]) && isNumber(command[2])) {
                int i = stoi(command[1]);
                int p = stoi(command[2]);
                char t = command[3][0];
                
                // Play card on player 1
                if (p == 1) {
                    if (game.getIsPlayer1Turn()) {
                        // Player1's turn to play on self
                        game.getPlayer1()->playCardHasTarget(i, game.getPlayer1(), t);
                    } else {
                        // Player2's turn to play on player 1
                        game.getPlayer2()->playCardHasTarget(i, game.getPlayer1(), t);
                    }

                // Play card on player 2
                } else if (p == 2) {
                    if (game.getIsPlayer1Turn()) {
                        // Player1's turn to play on player2
                        game.getPlayer1()->playCardHasTarget(i, game.getPlayer2(), t);
                    } else {
                        // Player2's turn to play on self
                        game.getPlayer2()->playCardHasTarget(i, game.getPlayer2(), t);
                    } // if
                } // if
            
            // Given command play i, the i-th card in hand is played
            } else if ((command.size() == 2) && isNumber(command[1])) {
                int i = stoi(command[1]);

                if (game.getIsPlayer1Turn()) {
                    // Player1's turn to play card i
                    game.getPlayer1()->playCardNoTarget(i, game.getPlayer2());
                } else {
                    // Player2's turn to play card i
                    game.getPlayer2()->playCardNoTarget(i, game.getPlayer1());
                }  // if  
            } // if

        // Use command  
        } else if ( firstCommand == "use" ) {

            std::string minionName = "";

            // Given command use i p t,
            // the i-th card on board is used on the t-th minion or ritual of player p
            // Note: use i p t is called by NOVICE PYROMANCER
            if ((command.size() >= 4) && isNumber(command[1]) && isNumber(command[2]) && isNumber(command[3])) {
                int i = stoi(command[1]);
                int p = stoi(command[2]);
                int t = stoi(command[3]);

                // Get the name of the minion of interest 
                if (game.getIsPlayer1Turn()) {
                    minionName = game.getPlayer1()->getElemFromBoard(i)->getName();
                } else {
                    minionName = game.getPlayer2()->getElemFromBoard(i)->getName();
                }

                // Only novice pyromancer can use the use i p t command
                if (minionName != MIN_NP) {
                    throw SorceryException("This card cannot be used with the 'use i p t' command.");
                }

                // Use card on player 1
                if (p == 1) {
                    if (game.getIsPlayer1Turn()) {
                        // Player1's turn to use on self
                        game.getPlayer1()->getElemFromBoard(i)->useCardOnCard(game.getPlayer1(), game.getPlayer1(), t);
                    } else {
                        // Player2's turn to use on player 1
                        game.getPlayer2()->getElemFromBoard(i)->useCardOnCard(game.getPlayer2(), game.getPlayer1(), t);
                    }
                // Use card on player 2
                } else if (p == 2) {
                    if (game.getIsPlayer1Turn()) {
                        // Player1's turn to use on player 2
                        game.getPlayer1()->getElemFromBoard(i)->useCardOnCard(game.getPlayer1(), game.getPlayer2(), t);
                    } else {
                        // Player2's turn to use on self
                        game.getPlayer2()->getElemFromBoard(i)->useCardOnCard(game.getPlayer2(), game.getPlayer2(), t);
                    }
                } // if
            
            // Given command use i, the i-th card on board is used
            } else if ((command.size() == 2) && isNumber(command[1])) {
                int i = stoi(command[1]);

                // Based on the active player, we call a summoner's function
                if (game.getIsPlayer1Turn()) {
                    minionName = game.getPlayer1()->getElemFromBoard(i)->getName();
                    if ((minionName == MIN_AS) || (minionName == MIN_MS)) {
                        // Player1's turn to use card i
                        game.getPlayer1()->getElemFromBoard(i)->useCardOnCard(game.getPlayer1(), game.getPlayer2(), i);
                    }
                    
                } else {
                    minionName = game.getPlayer2()->getElemFromBoard(i)->getName();
                    if ((minionName == MIN_AS) || (minionName == MIN_MS)) {
                        // Player2's turn to use card i
                        game.getPlayer2()->getElemFromBoard(i)->useCardOnCard(game.getPlayer2(), game.getPlayer1(), i);
                    } // if
                } // if
            } // if
                
        // Inspect command
        } else if ( firstCommand == "inspect" ) {

            // Given command inspect i, we print out the i-th minion
            if ((command.size() >= 2) && isNumber(command[1])) {
                int i = std::stoi(command[1]);

                // Based on the active player, we print the respective i-th minion
                if (game.getIsPlayer1Turn()) {
                    game.getPlayer1()->printMinionInfo(i);
                } else {
                    game.getPlayer2()->printMinionInfo(i);
                } // if
            } // if
            
        // Hand command
        } else if ( firstCommand == "hand" ) {

            // Based on which player's turn it is, the respective hand is printed
            if (game.getIsPlayer1Turn()) {
                game.getPlayer1()->displayPlayerHand();
            } else {
                game.getPlayer2()->displayPlayerHand();
            }
        
        // Board command
        } else if ( firstCommand == "board") {

            game.displayBoard();
        
        // None of the above command
        } else if ( firstCommand != "" ) {

            throw SorceryException("Command not recognized. Try again please...");
        }

    // Error catching
    } catch ( SorceryException & e ) {
        std::cerr << e.message << std::endl;
    } catch (std::invalid_argument & e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        throw SorceryException("Something went wrong...");
    } // catch

    return endOfGame;
}


int main(int argc, char **argv) {
    try {
        std::cin.exceptions(std::ios::eofbit|std::ios::failbit);

        // Set the positions of all the flags and files provided in the command line
        int * args = argPositions(argc,argv);

        // Sets the deck files of the players
        std::string deck1file = (args[0] != NO_PARAM) ? argv[ args[0] ] : DEFAULT_DECK;
        std::string deck2file = (args[1] != NO_PARAM) ? argv[ args[1] ] : DEFAULT_DECK;

        // Sets if we are in testing mode
        bool isTestingMode = (args[3] != NO_PARAM) ? true : false;

        // Sets the input files if one exists
        std::string inputFile;
        std::ifstream initFile;
        if (args[2] != NO_PARAM) {
            inputFile = argv[ args[2] ];
            initFile.open(inputFile);
        }

        std::string cmd = "";
        std::vector<std::string> names;
        
        // Create players either from init file or stdin
        for (int i = 1; i < 3; i++) {
            std::cout << "Player " << i << " Name: " << std::endl;

            // Read from file / input
            if (std::getline(initFile,cmd)) {
                names.push_back(cmd);
                std::cout << cmd << std::endl;
            } else {
                getline(std::cin, cmd);
                names.push_back(cmd);
            }
        }

        // Create two players
        Player * player1 = new Player(names[0], 1);
        Player * player2 = new Player(names[1], 2);

        // We made respective decks for each player
        player1->makeDeck(deck1file, isTestingMode);
        player2->makeDeck(deck2file, isTestingMode);
        
        // A game board is made with the 2 players
        PlayBoard game{player1, player2, isTestingMode};

        // Draws 5 initial cards for both players
        player1->beginningDrawHand();
        player2->beginningDrawHand();

        cmd = "";
        std::vector<std::string> command;


        bool endOfGame = false;

        // Init file Game Loop

        // The init file is read from, if provided and good
        while (std::getline(initFile, cmd) && !endOfGame) {
            try {
                // Reset command
                command.clear();

                // Split new command
                splitCommand(cmd, command);

                // Apply command on game
                endOfGame = runGameLoop(game,command);
            } catch (SorceryException & e) {
                std::cerr << e.message << std::endl;
            }
        } // while

        initFile.close();

        // User input Game Loop

        while (!endOfGame) {
            try {
                // Reset command
                command.clear();

                // Read new command
                std::getline(std::cin, cmd);

                // Split command
                splitCommand(cmd, command);

                // Apply command on game
                endOfGame = runGameLoop(game,command);
            } catch (SorceryException & e) {
                std::cerr << e.message << std::endl;
            }
        } // while

        // Freeing card dictionary 
        for (auto i = cardDict.begin(); i != cardDict.end(); i++) {
            // Helper to call dtor of respective card type in the map
            player1->pseudoDtor(i->second);
        }
        cardDict.clear();
        
        // Freeing players
        delete player1;
        delete player2; 

    } catch (std::ios::failure &) {
        // Any I/O failure
        std::cout << "I/O Error" << std::endl;

    } catch (...) {
        // Unknown error - this line should never run
        std::cout << "Game Error" << std::endl;
    } // catch
} // main


