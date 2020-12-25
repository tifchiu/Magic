#include "ritual.h"
#include "cardconstants.h"
#include "sorceryException.h"

// Constructor for a Ritual Card
Ritual::Ritual(std::string name, std::string type, int cost, int activationCost, int charges, AbilityTriggered * triggered) :
    Card(name, type, cost), 
    charges{charges}, 
    activationCost{activationCost}, 
    triggered{triggered}
{}

// Copy a Ritual Card
Ritual::Ritual (Ritual * other) : 
    Card(other->getName(), other->getCardType(), other->getCost()),
    charges{other->getCharges()},
    activationCost{other->getACost()}, 
    triggered{other->getTriggered()}
{}


// Destructor
Ritual::~Ritual() {

}

// return a reference to the Actual Triggered Ability
AbilityTriggered & Ritual::getActualTriggered(){
    return *triggered;
}


// return the number of Charges in the Triggered Ability
int Ritual::getCharges(){
    return charges;
}

// set the number of Charges in the Ritual Card
void Ritual::setCharges(int charges){
    this->charges = charges;
}

// return the Activation Cost of the Ritual Card
int Ritual::getACost(){
    return activationCost;
}

// set the Activation Cost of the Ritual Card
void Ritual::setACost(int aCost) {
    this->activationCost = aCost;
}

// set the Triggered Ability of the Ritual Card
void Ritual::setTriggered(AbilityTriggered * ptr) {
    triggered = ptr;
}


// return the Triggered Ability of the Ritual Card
AbilityTriggered * Ritual::getTriggered() {
    return triggered;
}


// use the Ritual Card (no target)
void Ritual::useCard(Player *activePlayer, int pos) {
    int boardSize = activePlayer->getBoardSize();
    
    // Math for charges and cost
    if (activationCost > charges) {
        throw SorceryException("A ritual was triggered, but it did not have enough charges.");
    } else {
        charges -= activationCost;
    }
    
    // Apply function according to ritual
    
    if (getName() == RIT_DARRIT) {
        activePlayer->setMagic(activePlayer->getMagic() + 1);

    } else if (getName() == RIT_AUROPOW){
        
        int curAttack = activePlayer->getElemFromBoard(boardSize)->getAttack();
        int curDefence = activePlayer->getElemFromBoard(boardSize)->getDefence();
        activePlayer->getElemFromBoard(boardSize)->setAttack(curAttack + 1);
        activePlayer->getElemFromBoard(boardSize)->setDefence(curDefence + 1);

    } else if (getName() == RIT_STAND){
        
        activePlayer->discardFromBoard(boardSize, true);

    } else { 
        throw SorceryException("Ritual DNE in database.");
    }
}

// display the Ritual Card
std::vector<std::string> Ritual::displayCard(){
    return display_ritual(getName(), getCost(), activationCost, triggered->getDescription(), charges);
}


