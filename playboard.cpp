#include <fstream>
#include <iostream>
#include "playboard.h"
#include "sorceryException.h"
#include "cardconstants.h"


PlayBoard::PlayBoard(Player * player1, Player * player2, bool isTestMode) :
    player1{player1}, 
    player2{player2}, 
    isTesting{isTestMode}, 
    isPlayer1Turn{true}
{}


Player * PlayBoard::getPlayer1() {
    return player1;
}

Player * PlayBoard::getPlayer2() {
    return player2;
}

bool PlayBoard::hasWon() {
    // Build output
    std::string ret = "[SORCERY]: ";
    
    // Current win is false
    bool hasWon = false;
    
    // Check if player's are dead
    if (player2->getHp() <= 0) {
        ret += player1->getName() + " has beat " + player2->getName() + "!";
        hasWon = true;

    } else if (player1->getHp() <= 0 ){
        ret += player2->getName() + " has beat " + player1->getName() + "!";
        hasWon = true;
    }

    // A player has won
    if (hasWon) {
        std::cout << ret << std::endl;
    }

    return hasWon;
}

void PlayBoard::hasTie() {
    std::cout << "[SORCERY]: The game has ended in a draw." << std::endl;
}

void PlayBoard::changeTurnEnd() {
    // Check which player is the activePlayer
    Player * activePlayer = (isPlayer1Turn) ? player1 : player2;

    // Apply notification
    activePlayer->notifyMinions(nullptr, State::EndOfTurn);

    // Change turn
    isPlayer1Turn = !isPlayer1Turn;
}

void PlayBoard::changeTurnStart() {
    // Check which player is active
    Player * activePlayer = (isPlayer1Turn) ? player1 : player2;
    std::cout << "[SORCERY]: " << activePlayer->getName() << ", it is your turn:" << std::endl;

    // Gain 1 magic
    activePlayer->setMagic(activePlayer->getMagic() + 1);
    
    // Draw card if necessary
    if (activePlayer->getHandSize() < 5) {
        try {
            activePlayer->drawFromDeckToHand();
        
        // Cannot draw from deck
        } catch (SorceryException & e) {
            std::cerr << e.message << std::endl;
        }
    }
    
    // Start of the turn effects occur
    if (activePlayer->hasRitual()) {
        auto ritual = player1->getRitual();
        if (ritual->getName() == RIT_DARRIT) {
            ritual->useCard(player1, 0);
        }
    }
    
    // Restore minion's action points
    activePlayer->restoreMinionAction();
}

bool PlayBoard::getIsPlayer1Turn() {
    return isPlayer1Turn;
}

bool PlayBoard::getIsTesting() {
    return isTesting;
}

void PlayBoard::printOutHelp() {
    
    std::cout << "Commands: help -- Display this message." << std::endl;
    std::cout << "          end -- End the current player's turn." << std::endl;
    std::cout << "          quit -- End the game." << std::endl;
    std::cout << "          attack minion other-minion -- Orders minion to attack other-minion." << std::endl;
    std::cout << "          attack minion -- Orders minion to attack the opponent." << std::endl;
    std::cout << "          play card [target-player target-card] -- Play card, optionally targeting target-card owned by target-player." << std::endl;
    std::cout << "          use minion [target-player target-card] -- Use minion's special ability, optionally targeting target-card owned by target-player." << std::endl;
    std::cout << "          inspect minion -- View a minion's card and all enchantments on that minion." << std::endl;
    std::cout << "          hand -- Describe all cards in your hand." << std::endl;
    std::cout << "          board -- Describe all cards on the board." << std::endl;
}

void PlayBoard::displayBoard() {
    // Top Border
    std::cout << player1->printBorderTopBot(true) << std::endl;

    // player1 outer row
    rowPrint(player1->displayCardOuterRow(1));

    // player 1 minions
    rowPrint(player1->displayCardsBoard());

    // centre graphics 
    for (auto line : player1->displayGameTitle()) {
        std::cout << line << std::endl;
    }

    // player2 minions
    rowPrint(player2->displayCardsBoard());

    // player 2 outer row
    rowPrint(player2->displayCardOuterRow(2));

    // Bot border
    std::cout << player1->printBorderTopBot(false) << std::endl;

}

void PlayBoard::rowPrint(std::vector<std::vector<std::string>> row) {
    // Prints out a row of cards
    for (int a = 0; a < HEIGHT; ++a){

        // Left border
        std::cout << player1->printBorderSide();

        // Cards
        for (int j = 0; j < FULL_BOARD; j++){
            std::cout << row[j][a];
        }

        // Right border
        std::cout << player1->printBorderSide() << std::endl;
    }
}
