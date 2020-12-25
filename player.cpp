#include "player.h"
#include "cardconstants.h"
#include "ascii_graphics.h"
#include "carddictionary.h"
#include "cardconstants.h"
#include "sorceryException.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <utility>
#include <chrono>

// see .h for detailed documentation

Player::Player(std::string name, int num) :
    name{name},
    playerNum{num}
{
    hp = HP_START;
    magic = MAGIC_START;
}

Player::~Player() {

    // Note: This can probably be simplified with helper functions, but
    // let's not play with iterators and memory. Also, Minions need to be deleted
    // in a unique way.

    // Free deck
    for (auto i = deck.begin(); i != deck.end(); i++) {
        if ((*i)->getCardType() == TYPE_M) {
            auto delMin = static_cast<Minion*>(*i);
            delete &delMin->getActualActivated();
            delete &delMin->getActualTriggered();
        }
        delete *i;
        *i = nullptr;
    }

    // Free ritual
    for (auto i = ritual.begin(); i != ritual.end(); i++) {
        delete *i;
        *i = nullptr;
    }

    // Free graveyard
    for (auto i = graveyard.begin(); i != graveyard.end(); i++) {
        auto delMin = static_cast<Minion*>(*i);
        delete &delMin->getActualActivated();
        delete &delMin->getActualTriggered();
        delete *i;
        *i = nullptr;
    }    

    // Free board
    for (auto i = board.begin(); i != board.end(); i++) {
        auto delMin = static_cast<Minion*>(*i);
        delete &delMin->getActualActivated();
        delete &delMin->getActualTriggered();
        
        delete *i;
        *i = nullptr;
    }

    // Free hand
    for (auto i = hand.begin(); i != hand.end(); i++) {
        if ((*i)->getCardType() == TYPE_M) {
            auto delMin = static_cast<Minion*>(*i);
            delete &delMin->getActualActivated();
            delete &delMin->getActualTriggered();
        }
        
        delete *i;
        *i = nullptr;
    }
}

void Player::pseudoDtor(Card * toDestroy) {
    // Free each value in the map

    // Minion to free
    if (toDestroy->getCardType() == TYPE_M) {
        auto mapThing = static_cast<Minion*>(toDestroy);
        delete &mapThing->getActualActivated();
        delete &mapThing->getActualTriggered();
        delete mapThing;

        mapThing = nullptr;

    // Spell to free
    } else if (toDestroy->getCardType() == TYPE_S) {
        auto mapThing = static_cast<Spell*>(toDestroy);
        delete &mapThing->getActualActivated();
        delete mapThing;

        mapThing = nullptr;

    // Ritual to free
    } else if (toDestroy->getCardType() == TYPE_R) {
        auto mapThing = static_cast<Ritual*>(toDestroy);
        delete &mapThing->getActualTriggered();
        delete mapThing;

        mapThing = nullptr;

    // Enchantment to free
    } else {
        auto mapThing = static_cast<Enchantment*>(toDestroy);
        delete mapThing;

        mapThing = nullptr;
    }
}

bool Player::hasRitual() {
    return (ritual.size() > 0);
}

int Player::getBoardSize(){
    return board.size();
}

int Player::getRitualSize(){
    return ritual.size();
}

int Player::getGraveyardSize(){
    return graveyard.size();
}
std::string Player::getName() {
    return name;
}

void Player::addCardToBoard(Card* newCard) {
    board.push_back(newCard);
}


void Player::setName(std::string newName) {
    name = newName;
}

int Player::getHp() {
    return hp;
}

void Player::setHp(int num) {
    hp = num;
}

int Player::getMagic() {
    return magic;
}

void Player::setMagic(int num) {
    magic = num;
}

void Player::changeHp(int change) { 
    hp += change;
}

void Player::changeMagic(int change) {
    magic += change;
}

void Player::useMinionToAttack(Player * opponent, int myMinion, int minionToAttack) {

    // Active player minion at myMinion will attack opponent's minion at miniontoAttak
    auto myMin = getElemFromBoard(myMinion);

    // Indicate if the minion attacked is dead
    bool myMinionIsStillAlive = myMin->attackMinion(opponent, minionToAttack);
    
    // Check if the minion is dead
    if (!myMinionIsStillAlive) {
        // Remove via index search
        removeFromBoardToGraveYard(findIndexInBoard(myMin));
    }
}

void Player::restoreMinionAction() {
    // Gives every minion on the board of the active player a new action point
    for (auto & onBoard : board) {

        auto minion = static_cast<Minion*>(onBoard);
        int ap = minion->getActionPoints();

        // Not full action points means you can get one more
        if (ap < 1) {
            minion->setActionPoints(ap + 1);
        }
    }
}

void Player::notifyMinions(Subject * whoNotified, State stateOfGame) {
    for (auto & onBoard : board) {
        auto minion = static_cast<Minion*>(onBoard);

        // For every minion on the board, if the triggered AbilityTriggered by game condition, notify
        if ((minion->getTriggered() != nullptr) && (minion->getTriggered()->getCondition() == stateOfGame)) {
            minion->notify(whoNotified, stateOfGame);
        }   
    }
}

Minion * Player::getElemFromBoard(int index) {
    int lastBoardIndex = board.size();
    if ((index > lastBoardIndex) || (index <= 0)) {
        throw SorceryException("Bad index!!! Please try again.");
    }
    // minus one as command line arg index ranges from 1 to 5 as opposed to 0 to 4
    return static_cast<Minion*>(board[index-1]);
}

// Plays the i-th card from the active player's hand on the t-th card of player # p
void Player::playCardHasTarget(int i, Player * p, char t) {
    // We can use the useCard methods here
    int lastCardInHand = hand.size();

    // Invalid i (greater than hand size or negative) means do nothing
    if ((i > lastCardInHand) || (i <= 0)) {
        throw SorceryException("Command read in bad index. Please try again.");
    }
    
    auto handFind = hand.begin() + i - 1;

    std::string cardType = (*handFind)->getCardType();
    std::string cardName = (*handFind)->getName();
    int cost = (*handFind)->getCost();
        
    // check if player has enough magic. Magic is deducted at the end 
    if (magic < cost) {
        throw SorceryException("Player does not have enough magic for this card.");
    }
    
    int target = t - '0';

    // Enchantment
    if (cardType == TYPE_E) {

        bool enchantAdded = p->getElemFromBoard(target)->addEnchantment(i, this, target);
        
        // Destroy if should be, otherwise just remove
        discardFromHand(i, !enchantAdded);

    // Spell
    } else if (cardType == TYPE_S) {

        // If target is a ritual
        if (t == 'r') {
            auto spellIt = std::find(cardsWithTarget.begin(), cardsWithTarget.end(), cardName);

            // Found => has target
            if (spellIt != cardsWithTarget.end()) {
                auto spellWithTarget = static_cast<Spell*>(*handFind);

                // Convert r to -1 to tell useCardOnCard to use on 
                spellWithTarget->useCardOnCard(this, p, i, -1); // -1 means ritual

            } else {
                throw SorceryException("This spell must use the play command with three arguments!");
            }

            // Destroys spell from hand - will not run if thrown
            discardFromHand(i, true);

        // Target is not a ritual
        } else {
            // Spell i on target player p and 1 to 5 t
            auto spellIt = std::find(cardsWithTarget.begin(), cardsWithTarget.end(), cardName);

            // Found => has target
            if (spellIt != cardsWithTarget.end()) {

                // Call this spell which has target
                auto spellWithTarget = static_cast<Spell*>(*handFind);
                spellWithTarget->useCardOnCard(this, p, i, target);

            // Not a spell with a target = wrong command given
            } else {
                throw SorceryException("This spell must use the play command with one argument!");
            }

            // Destroys spell from hand - will not run if thrown
            discardFromHand(i, true);
        }
    
    } else {
        throw SorceryException("The play command given should not be used on this card.");
    }

    magic -= cost;
}

void Player::attachObserversToNewMinion(Card * newMinion) {
    for (auto & onBoard : board) {
        // Adds minion to the observer of newMin
        auto minion = static_cast<Minion*>(onBoard);
        auto newMin = static_cast<Minion*>(newMinion);
        newMin->attach(minion);
    }
}

void Player::attachNewMinionToObservers(Card * newMinion) {
    for (auto & onBoard : board) {
        // Adds the newMin to the minion observers
        auto minion = static_cast<Minion*>(onBoard);
        auto newMin = static_cast<Minion*>(newMinion);
        minion->attach(newMin);
    } 
}


 
// Plays the i-th card in the active player's hand
void Player::playCardNoTarget(int i, Player * opponent) {
    int lastHandIndex = hand.size();
    
    // Check if valid index
    if ((lastHandIndex >= i) && (i > 0)) {
        
        auto handFind = hand.begin() + i - 1;

        // Get card Info
        std::string cardType = (*handFind)->getCardType();
        std::string cardName = (*handFind)->getName();
        
        int cost = (*handFind)->getCost();
        
        // check if player has enough magic
        if (magic < cost) {
            throw SorceryException("Player does not have enough magic for this card.");
        }
        
        // Minion
        if ((cardType == TYPE_M)) {
            
            //attach all minions on the active player's board to new min observers.
            // Lets the new minion know "these are your teammates"
            attachObserversToNewMinion(*handFind); 

            // add mew minion to board
            board.push_back(*handFind);

            // attach new minion to be an observer of all minions on the active player's board (including itself)
            // Lets the acitve player's minions know "this is your new teammate"
            attachNewMinionToObservers(*handFind);
            
            // attach all opponent minions to new minion observers
            // Lets the new minion know "these are your opponents"
            opponent->attachObserversToNewMinion(*handFind);
            
            // attach new minion to be an observer of all opponent's minions
            // Let's the opponent's minions know "this is your new opponent"
            opponent->attachNewMinionToObservers(*handFind);

            // Clears hand
            hand.erase(handFind);

            // APNAP : Check this->ritual before checking opponent ritual
            if (hasRitual()) {
                auto ritual = getRitual();

                // Use right ritual
                if (ritual->getName() == RIT_AUROPOW) {
                    ritual->useCard(this, 0);
                } else if (ritual->getName() == RIT_STAND) {
                    ritual->useCard(this, 0);
                }
            }

            // Opponent ritual
            if (opponent->hasRitual()) {
                auto opponentRit = opponent->getRitual();
                // Use ritual
                if (opponentRit->getName() == RIT_STAND) {
                    opponentRit->useCard(this, 0);
                }
            }

            // specific to fire elemental. Broadcast a notification that a minion has entered play
            opponent->notifyMinions(static_cast<Minion*>(*handFind), State::MinionEnterPlay);


        // Ritual
        } else if (cardType == TYPE_R) {

            // Adds ritual to board
            removeRitual(false);
            ritual.push_back(*handFind);
            hand.erase(handFind);
        
        // Spell
        } else if (cardType == TYPE_S) {
            auto spellWithTarget = static_cast<Spell*>(*handFind);

            try {
                // Apply spell cards that do not have a target
                if (cardName == SP_BLIZ) {
                    spellWithTarget->useCardOnCard(this, opponent, i, i);
                } else {
                    spellWithTarget->useCard(this, i);
                }

                // Remove spell
                delete spellWithTarget;
                hand.erase(handFind);

            } catch (SorceryException & e) {
                throw e;
            }
        } else {
            throw SorceryException("Perhaps the play i p t command might be of interest?");
        }

        // If any throws occurred, this will not run
        magic -= cost;
    }
}

void Player::drawFromDeckToHand() {

    int lastCardIndex = deck.size();

    int cardsHeld = hand.size();

    // A full hand means we cannot draw
    if (cardsHeld >= FULL_HAND) {
        throw SorceryException("You cannot draw. Your hand is full!");
    }

    // If valid last card index
    if (lastCardIndex != 0) {
        auto deckBack = deck.begin();

        // Move the last card in the deck vector to the end of the hand vector
        hand.push_back(*deckBack);

        deck.erase(deckBack);
    } else {
        throw SorceryException("No more cards left in the deck to draw... good luck.");
    }

}

void Player::discardFromHand(int i, bool toDestroy) {
    int lastCardInHand = hand.size();
 
    // Invalid i (greater than hand size or negative) means do nothing
    if ((i > lastCardInHand) || (i <= 0)) {
        throw SorceryException("Your hand index is not valid.");
    }

    // If i-th index has valid card
    if (lastCardInHand >= i) {
        auto handIterator = hand.begin() + i - 1;

        // Destroy card if needed
        if (toDestroy) {
            if ((*handIterator)->getCardType() == TYPE_M) {
                pseudoDtor(*handIterator);
            } else {
                delete *handIterator;
                *handIterator = nullptr;
            }
        }
        
        // Remove from hand
        hand.erase(handIterator);

        
    }
}


void Player::discardFromBoard(int i, bool toDestroy){
    int lastCardonBoard = board.size();

    // Invalid i (greather than hand size or negative) means do nothing 
    if ((i > lastCardonBoard) || (i <= 0)){
        throw SorceryException("Your board index is not valid.");
    }

    // If i-th index has valid card
    if (lastCardonBoard >= i){
        auto boardIterator = board.begin() + i - 1;

        // Destroy if needed
        if (toDestroy) {
            pseudoDtor(*boardIterator);
            board.erase(boardIterator);

        }
        
        board.erase(boardIterator);
    }
}

void Player::removeFromBoardToGraveYard(int i) {
    int lastCardOnBoard = board.size();
    
    // Invalid i (greater than board size or negative) means do nothing
    if ((i > lastCardOnBoard) || (i <= 0)) {
        throw SorceryException("Your graveyard index is not valid.");
    }

    // If i-th index has valid card
    if (lastCardOnBoard >= i) {
        auto boardIterator = board.begin() + i - 1;

        if ((*boardIterator)->getDefence() > 0) {
            // This should never run
            throw SorceryException("We apologize. This should not have happened.");
        }

        // Move the i-th card on the board vector to the end of the graveyard
        auto min = static_cast<Minion*>(*boardIterator);
        min->clearEnchantments();
        graveyard.push_back(std::move(*boardIterator));

        board.erase(boardIterator);
    }
}

void Player::minionToHand(int i) {
    int lastCardOnBoard = board.size();
    int lastCardInHand = hand.size();

    // If we have a valid index to move a minion from board to hand
    if ((i >= 1) && (i <= lastCardOnBoard) && (lastCardInHand < FULL_HAND)) {
        auto boardIterator = board.begin() + i - 1;

        // Move the i-th card on the board vector to the end of the hand vector
        hand.push_back(std::move(*boardIterator));

        board.erase(boardIterator);

    } else {
        throw SorceryException("Player's hand is full.");
    }
}


void Player::removeRitual(bool toDestroy){
    int size = ritual.size();

    if (size != 0) {
        // Destory if needed
        if (toDestroy) {
            delete ritual[0];
        }
        ritual.clear();
    }
}

void Player::drawTopFromGraveYard() {

    // Get last card in graveyard
    int lastCardInGraveYard = graveyard.size() - 1;
    auto graveIterator = graveyard.begin() + lastCardInGraveYard;

    if (lastCardInGraveYard >= 0) {             // Is valid place to take from graveyard
        if (board.size() == FULL_BOARD) {   
            if (hand.size() == FULL_HAND) {     // Hand is full means we put into top of deck
                deck.push_back(std::move(*graveIterator));
            } else {                            // Board is full means we put into end of hand
                hand.push_back(std::move(*graveIterator));
            }
        } else {
            board.push_back(std::move(*graveIterator));
        }
        graveyard.erase(graveIterator);
    }
}

void Player::beginningDrawHand() {
    try {
        // Draws 5 cards
        for (int times = 0; times < FULL_HAND; times++) {
            this->drawFromDeckToHand();
        }
    } catch (SorceryException & e) {
        std::cerr << e.message << std::endl;
    }
}

Enchantment * Player::getEnchantmentFromHand(int index) {
    int lastHandIndex = hand.size();

    // Invalid i (greater than hand size or negative) means do nothing
    if ((index > lastHandIndex) || (index <= 0)) {
        throw SorceryException("Invalid hand index"); 
    }
    // minus one as index ranges from 1 to 5 instead of 0 to 4
    return static_cast<Enchantment*>(hand[index-1]);
}

Ritual * Player::getRitual(){

    if (ritual.size() == 0){
        throw SorceryException("Nothing in the Ritual vector");
    }
    return static_cast<Ritual*>(ritual[0]);

}

Minion * Player::getMinionFromGraveyard(int index){
    int lastMinionIndex = graveyard.size();
    if ((index > lastMinionIndex) || (index <= 0)){

        throw SorceryException("Invalid Graveyard index.");
    }
    return static_cast<Minion*>(graveyard[index - 1]);
}

Card * Player::getElemFromHand(int index) {
    int lastCardIndex = hand.size();

    if ((index > lastCardIndex) || (index <= 0)){

        throw SorceryException("Attempted to access a non-existent Card from your hand.");
    }
    return static_cast<Card*>(hand[index - 1]);
}

void Player::shuffleDeck(std::vector<Card*> &newDeck) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(newDeck.begin(), newDeck.end(), std::default_random_engine(seed));
}

void Player::makeDeck(std::string filename, bool isTesting) {

    std::ifstream test (filename);
    std::ifstream inputFile;

    // Check if the provided deck file is in a good state. If not, we read from default.deck instead.
    if (!test.good()) {
        inputFile.open("default.deck");
        std::cerr << "[SORCERY]: The deck provided for " << getName() << " was not valid. We have provided you with the default Sorcery deck." << std::endl;
    } else {
        std::cerr << "[SORCERY]: The deck provided for " << getName() << " was loaded successfully!" << std::endl;
        inputFile.open(filename);
    }

    try {
        std::string line = "";
        std::string untrimmedLine = "";

        // For every line in the file, read it in
        
        while( getline(inputFile, untrimmedLine) ) {

            line = untrimmedLine;
            int len = untrimmedLine.length();

            // Trim the input in case some files like to have ending spaces
            for (int x = len - 1; x >=0; x--) {
                if (untrimmedLine[x] != ' ') {
                    line = untrimmedLine.substr(0, x + 1);
                    x = -1;
                }
            }

            // Find if this card is a valid card
            auto finder = std::find(allCards.begin(), allCards.end(), line);

            // line is a valid card name
            if (finder != allCards.end()) {

                if (cardDict[line]->getCardType() == TYPE_E) {  // Enchantment

                    auto newThing = new Enchantment(static_cast<Enchantment*>(cardDict[line]));
                    deck.push_back(newThing);

                } else if (cardDict[line]->getCardType() == TYPE_M) {   // Minion
                    
                    auto newThing = new Minion(static_cast<Minion*>(cardDict[line]));

                    // If this minion has any triggers, we need to invoke the appropriate copy ctor
                    auto find = std::find(cardsWithTrigger.begin(), cardsWithTrigger.end(), line);

                    // AbilityTriggered
                    if (find != cardsWithTrigger.end()) {
                        // Add appropriate AbilityTriggered
                        auto newTrig = new AbilityTriggered(newThing->getActualTriggered());
                        newThing->setTriggered(newTrig);

                    // Potential AbilityActivated
                    } else {
                        find = std::find(cardsWithActivated.begin(), cardsWithActivated.end(), line);

                        // Add appropriate AbilityActivated
                        if (find != cardsWithActivated.end()) {
                            auto newAct = new AbilityActivated(newThing->getActualActivated());
                            newThing->setActivated(newAct);
                        }
                    } // if

                    // Add card to deck
                    deck.push_back(newThing);

                } else if (cardDict[line]->getCardType() == TYPE_S) {   // Spell

                    auto newThing = new Spell(static_cast<Spell*>(cardDict[line]));
                    deck.push_back(newThing);

                } else if (cardDict[line]->getCardType() == TYPE_R) {   // Ritual

                    auto newThing = new Ritual(static_cast<Ritual*>(cardDict[line]));
                    deck.push_back(newThing);
                } // if

                // Set owner of the new card to the current player
                deck.back()->setOwner(this);

            } else {
                std::cout << "[SORCERY]: Illegal thing exists in file:  " << line << std::endl;
            } // if
        } // while
    } catch (...) {
        throw SorceryException("The deck could not be generated");
    }
    // Shuffle deck unless in testing mode
    if (!isTesting) {
        shuffleDeck(deck);
    }
}

int Player::getDeckSize() {
    return deck.size();
}

int Player::getHandSize()  {
    return hand.size();
}

std::string Player::printBorderSide() {
    return EXTERNAL_BORDER_CHAR_UP_DOWN;
}

std::vector<std::string> Player::displayCardPlayer(){
    return display_player_card(playerNum, name, hp, magic);
}


std::vector<std::vector<std::string>> Player::displayCardsBoard() {
    std::vector<std::vector<std::string>> output;
    
    // Create minions on board
    int minionNum = board.size();
    for (auto & minionOnBoard : board) {
        output.push_back(minionOnBoard->displayCard()); 
    }

    // Create empty minion slots
    for (int difference = minionNum; FULL_BOARD > difference; difference++) {
        output.push_back(CARD_TEMPLATE_BORDER);
    }

    return output;
}

std::vector<std::vector<std::string>> Player::displayCardOuterRow(int playerNum) {

    std::vector<std::vector<std::string>> output;
    std::vector<std::string> bordertemp = CARD_TEMPLATE_BORDER;
    std::vector<std::string> emptytemp = CARD_TEMPLATE_EMPTY;

    // Player 1
    if (playerNum == 1) {
        output.push_back(bordertemp);
        
        output.push_back(emptytemp);
        
        output.push_back(displayCardPlayer());
        
        output.push_back(emptytemp);

        if (ritual.size() == 0) {
            
            output.push_back(bordertemp);
        } else {
            auto & ritualInGame = ritual[0];
            output.push_back(ritualInGame->displayCard());
        }

    // Player 2
    } else {
        if (ritual.size() == 0) {
            
            output.push_back(bordertemp);
        } else {
            auto & ritualInGame = ritual[0];
            output.push_back(ritualInGame->displayCard());
        }
        
        output.push_back(emptytemp);
        output.push_back(displayCardPlayer());
        output.push_back(emptytemp);
        output.push_back(bordertemp);
    }
    
    return output;
}

std::string Player::printBorderTopBot(bool top) {
    // Determine if we are printing top or bot
    std::string output = top ? EXTERNAL_BORDER_CHAR_TOP_LEFT : EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;

    // Top border
    for (int x = 0; x < CARD_HORIZ_LEN * FULL_BOARD; x++){
        output += EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
    }
    
    // Add corner accordingly
    output += top ? EXTERNAL_BORDER_CHAR_TOP_RIGHT : EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT;
    return output;
}

void Player::printMinionInfo(int i) {
    int boardSize = board.size();

    // See if good index
    if ((boardSize > 0) && (i <= boardSize)) {
        // Print out enchantments for minion i
        getElemFromBoard(i)->printOutEnchantments();
    } else {
        throw SorceryException("Invalid index for minion inspection.");
    }
}

std::vector<std::string> Player::displayGameTitle(){
    std::vector<std::string> titleTemp = CENTRE_GRAPHIC;
    return titleTemp;
}

void Player::displayPlayerHand() {
    std::vector<std::vector<std::string>> output;
    
    int cardNum = hand.size();

    // No Cards
    if (cardNum == 0) {
        std::cout << "[Sorcery]: No cards in your hand." << std::endl;
        return;
    }

    for (auto & cardInHand : hand) {

        // Minion card
        if (cardInHand->getCardType() == TYPE_M) {

            auto cardOfInterest = static_cast<Minion*>(cardInHand);
            std::vector<std::string> cardOut = cardOfInterest->displayCard();

            output.push_back(cardOut);

        // Enchantment card
        } else if (cardInHand->getCardType() == TYPE_E) {

            auto cardOfInterest = static_cast<Enchantment*>(cardInHand);

            std::vector<std::string> cardOut = cardOfInterest->displayCard();

            output.push_back(cardOut);

        // Spell card
        } else if (cardInHand->getCardType() == TYPE_S) {

            auto cardOfInterest = static_cast<Spell*>(cardInHand);

            std::vector<std::string> cardOut = cardOfInterest->displayCard();

            output.push_back(cardOut);

        // Ritual card
        } else if (cardInHand->getCardType() == TYPE_R) {

            auto cardOfInterest = static_cast<Ritual*>(cardInHand);

            std::vector<std::string> cardOut = cardOfInterest->displayCard();

            output.push_back(cardOut);

        } else {
            // Should never run
            throw SorceryException("Encountered an invalid card type.");
        }
    }

    // Print out it all
    for (int a = 0; a < CARD_HEIGHT; ++a){
        for (int j = 0; j < cardNum; j++){
            std::cout << output[j][a];
        }
        std::cout << std::endl;
    }
}

int Player::findIndexInBoard(Minion * min) {
    int x = 1;

    // Finds minion pointer in board
    for (auto & check : board) {
        auto elem = static_cast<Minion*>(check);

        // Found
        if (min == elem) {
            return x;
        }
        x++;
    }
    
    return -1;
}

void Player::damageMinionsOnBoard(Player * activePlayer, Player * inactivePlayer, int damage) {
    // change health points for all minions on the active player's board
    for (auto & minion : board) {

        auto min = static_cast<Minion*>(minion);

        if (min->getDefence() > 0) {
            min->changeHealthPoints(damage);
        }
    }

    // iterate through board and remove dead minions

    for (auto it = board.begin(); it != board.end(); it++) {

        if ((*it)->getDefence() <= 0) {
            auto min = static_cast<Minion*>(*it);

            removeFromBoardToGraveYard(findIndexInBoard(min));
            // we have shifted the board and therefore must reset the iterator
            it = board.begin();
        }
   }
}

// same function as damageMinionsOnBoard
void Player::healMinionsOnBoard(int heal){
    for (auto & minion : board){
        auto min = static_cast<Minion*>(minion);
        min->changeHealthPoints(heal);
    }
}

