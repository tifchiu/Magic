#include "spell.h"
#include "player.h"
#include "enchantment.h"
#include "ritual.h"
#include "ability.h"
#include "cardconstants.h"
#include "ascii_graphics.h"
#include "sorceryException.h"


// Spell constructor 
Spell::Spell(std::string name, std::string type, std::string description, int cost, AbilityActivated * activated) : 
    Card(name, type, cost), 
    activated{activated}
{}

// get Actual Activated Ability
AbilityActivated & Spell::getActualActivated(){
    return *activated;
}

// Copy Constructor for a Spell
Spell::Spell (Spell * other) : 
    Card(other->getName(), other->getCardType(), other->getCost()),
    activated{other->getActivated()}
{}

// get the Activated Ability from the Spell Card
AbilityActivated * Spell::getActivated(){
    return activated;
}

// Destructor
Spell::~Spell() {}

// Set the Activated Ability of the Spell to ptr
void Spell::setActivated(AbilityActivated * ptr) {
    activated = ptr;
}


// Uses the Spell Card and applies its effects to a target
void Spell::useCardOnCard(Player *activePlayer, Player *affectedPlayer, int pos1, int pos2){
    // get the name of the card being used
    std::string nameOfCardUsed = activePlayer->getElemFromHand(pos1)->getName();
    // if the Card is Banish
    if (nameOfCardUsed == SP_BAN){
        if (pos2 == -1){ // denotes use of ritual card when calling play i p t
            // Remove ritual element from the vector 
            activePlayer->removeRitual(true);
        } else {
            // Destroy target minion from player 2's board
            affectedPlayer->discardFromBoard(pos2, true);
        } 
        // if the Card is Unsummon
    } else if (nameOfCardUsed == SP_UNSUM){
        // return Minion to target's hand
        affectedPlayer->minionToHand(pos2);
        // if the Card is Disenchant
    } else if (nameOfCardUsed == SP_DISEN){
        // Remove Enchantment from target player
        affectedPlayer->getElemFromBoard(pos2)->removeEnchantment();
        
    // if the Card is Blizzard
    } else if (nameOfCardUsed == SP_BLIZ){
        // Damage players from active Player
        activePlayer->damageMinionsOnBoard(activePlayer, affectedPlayer, -2);
        // Damage players from target Player
        affectedPlayer->damageMinionsOnBoard(activePlayer, affectedPlayer, -2);
    } else { // if Spell card is not identified, throw error
        throw SorceryException("This spell requires a target. Consider using the play i p t command.");
    }
}

// Uses a Spell Card and applies its effects (no target)
void Spell::useCard(Player *activePlayer, int pos) {
    std::string nameOfCardUsed = getName();

    // Recharge
    if (nameOfCardUsed == SP_RECH){
        int ritualSize = activePlayer->getRitualSize();

        // Has ritual?
        if (ritualSize == 0){
            throw SorceryException("Attempted to play Recarge on a player with no Ritual.");
        } else {
            // Apply ritual spell
            int curCharge = activePlayer->getRitual()->getCharges();
            int newCharge = curCharge + 3;
            activePlayer->getRitual()->setCharges(newCharge);
        }
    
    // Raise dead
    } else if (nameOfCardUsed == SP_RAIDEA){

        int graveyardSize = activePlayer->getGraveyardSize();

        if (graveyardSize == 0){
            throw SorceryException("No Minions in the graveyard, can not use spell Raise Dead");
        }

        // Set def to 1 for graveyard minions
        int newDef = 1;
        activePlayer->getMinionFromGraveyard(graveyardSize)->setDefence(newDef);
        activePlayer->drawTopFromGraveYard();
    

    } else {
        throw SorceryException("This spell has a target. Consider using the 'play i p t' command.");
    }
}

// Display a Spell Card from ascii_graphics 
std::vector<std::string> Spell::displayCard() {
    return display_spell(getName(), getCost(), activated->getDescription());
}
