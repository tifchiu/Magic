#include "minion.h"
#include "sorceryException.h"
#include "ascii_graphics.h"
#include "cardconstants.h"

Minion::Minion(std::string name, std::string cardType, int cost, int attack, 
        int defence, AbilityActivated * activated, AbilityTriggered * triggered):
    Card(name, cardType, cost, attack, defence), 
    silenced{false},
    actionPoints{0}, 
    activated{activated},
    triggered{triggered}
{}

Minion::Minion(std::string name, std::string cardType, int cost, int attack, int defence, AbilityActivated * activated) :
    Card(name, cardType, cost, attack, defence),
    silenced{false},
    actionPoints{0}, 
    activated{activated},
    triggered{nullptr}
{}

Minion::Minion(std::string name, std::string cardType, int cost, int attack, int defence, AbilityTriggered * triggered) :
    Card(name, cardType, cost, attack, defence),
    silenced{false},
    actionPoints{0}, 
    activated{nullptr},
    triggered{triggered}
{}

Minion::Minion(std::string name, std::string cardType, int cost, int attack, int defence) : 
    Card(name, cardType, cost, attack, defence),
    silenced{false},
    actionPoints{0}, 
    activated{nullptr},
    triggered{nullptr}
{}

Minion::Minion (Minion * other) : 
    Card(other->getName(), other->getCardType(), other->getCost(), other->getAttack(), other->getDefence()),
    silenced{false},
    actionPoints{0},
    activated{other->getActivated()},
    triggered{other->getTriggered()}
{}


Minion::~Minion(){
    clearEnchantments();
}


void Minion::setTriggered(AbilityTriggered * ptr) {
    triggered = ptr;
}

void Minion::setActivated(AbilityActivated * ptr) {
    activated = ptr;
}

int Minion::getActionPoints() {
    return actionPoints;
}

void Minion::setActionPoints(int x) {
    actionPoints = x;
}
  
AbilityActivated * Minion::getActivated() {
    return activated; 
}

AbilityTriggered * Minion::getTriggered() {
    return triggered; 
}

AbilityActivated & Minion::getActualActivated() {
    return *activated; 
}

AbilityTriggered & Minion::getActualTriggered() {
    return *triggered; 
}

int Minion::getEnchantmentsHistorySize(){
    return enchantmentHistory.size();
}

void Minion::clearEnchantments(){

    // Iterate thru enchants, and free the enchantment card
    for (auto i = enchantmentHistory.begin(); i != enchantmentHistory.end(); i++) {
        delete *i;
        *i = nullptr;
    }
    enchantmentHistory.clear();
}

// Minion does damage to the Player's basic health
void Minion::attackPlayer(Player * player){
    int hasMinions = player->getBoardSize();
    
    // changeHp function is in player
    if (actionPoints == 1) {
        
        // Cannot attack player if opponent has minions
        if (hasMinions) {

            std::cout << "[Sorcery]: There are minions in the way!" << std::endl;
            return;

        } else {
            // Attacks player 
            player->changeHp(-getAttack());
            actionPoints = 0;
        }

    // No action points
    } else {
        std::cout << "[Sorcery]: " << getName() << " does not have an action point." << std::endl;
        return;
    }
}


bool Minion::attackMinion(Player * opponent, int minionToAttack) {
    
    // Check if has action point to attack
    if (this->getActionPoints() != 1) {
        std::cout << "[Sorcery]: " << getName() << " does not have an action point." << std::endl;
        return true;
    }

    // The minion that is hurt
    auto hurtMinion = opponent->getElemFromBoard(minionToAttack);
    
    // Check if the hurt minion is alive
    bool attackedMinionIsStillAlive = hurtMinion->changeHealthPoints(-getAttack());

    // Remove if dead
    if (!attackedMinionIsStillAlive) {
        opponent->removeFromBoardToGraveYard(minionToAttack);
    }
    
    // Apply rebounded damage
    bool myMinionIsStillAlive = changeHealthPoints(-(hurtMinion->getAttack()));

    // Set action to finished
    actionPoints = 0;
    
    return myMinionIsStillAlive;
}

bool Minion::changeHealthPoints(int newPts){

    bool minionIsStillAlive = true;

    // Change health by factor of newPts
    setDefence(getDefence() + newPts);

    // Check if dead
    if (getDefence() <= 0) {

        // Minion is dead
        minionIsStillAlive = false;

        // Check if there is a potential triggered ability to use
        if ((getTriggered() != nullptr) && (getTriggered()->getCondition() == State::MinionExitPlay)) {
            notifyObservers(State::MinionExitPlay);
        }
    }
    return minionIsStillAlive;
}

bool Minion::getSilenced(){
    return silenced;
}

bool Minion::addEnchantment(int i, Player * currentPlayer, int target){
    // i is the position of enchantment on getEnchantmentFromHand
    // p is the position of minion on board of active player
    // add enchantment to history
    Enchantment * enchantToAdd = currentPlayer->getEnchantmentFromHand(i);
    bool toAdd = true;

    // Get info
    int newAttack = getAttack();
    int newDefence = getDefence();
    
    // Apply Giant Str enchant
    if (enchantToAdd->getName() == EN_GIASTR){
        newAttack += 2;
        newDefence += 2;

        this->setAttack(newAttack);
        this->setDefence(newDefence);

    // Apply Enrage enchant
    } else if (enchantToAdd->getName() == EN_ENR){
        newAttack *= 2;
        bool isStillALive = changeHealthPoints(-2);

        if (!isStillALive) {
            // Dead minion is to be removed
            currentPlayer->removeFromBoardToGraveYard(target);
        }

        this->setAttack(newAttack);

    // Apply Delay enchant
    } else if (enchantToAdd->getName() == EN_DEL){
        toAdd = false;
        actionPoints = -1;

    // Apply Magic Fatigue enchant
    } else if (enchantToAdd->getName() == EN_MAGFAT){

        if (activated == nullptr){
            return false;
        } else {
            // Effect activated ability cost
            activated->setCost(activated->getCost() + 2);
        }

    // Apply Silence enchant
    } else if (enchantToAdd->getName() == EN_SILEN){
        silenced = true;
    }

    // If not destroyed, add to enchantments
    if (toAdd) {
        enchantmentHistory.push_back(enchantToAdd);
    }

    return toAdd;

}

// this function is used for the Disenchant spell
void Minion::removeEnchantment(){

    // Check if there are any enchants
    if (enchantmentHistory.size() == 0) {
        std::cout << "[Sorcery]: You disenchanted a minion with no enchantments! What a waste..." << std::endl;
        return;
    }

    // Get last enchantment
    std::string topEnchantment = enchantmentHistory.back()->getName(); 

    int newAttack = getAttack();
    int newDefence = getDefence();

    // Undo Giant Str Enchant
    if (topEnchantment == EN_GIASTR){
    
        newAttack -= 2;
        newDefence -= 2;
        setAttack(newAttack);
        setDefence(newDefence);

    // Undo Enrage Enchant
    } else if (topEnchantment == EN_ENR){

        newAttack = newAttack / 2;
        newDefence = newDefence + 2;
        setAttack(newAttack);
        setDefence(newDefence);

    // Undo Delay
    } else if (topEnchantment == EN_DEL){

        actionPoints = 0;

    // Undo Magic Fatigue
    } else if (topEnchantment == EN_MAGFAT){

        activated->setCost(activated->getCost() - 2);

    // Undo Silence
    } else if (topEnchantment == EN_SILEN){

        silenced = false;
    }
    
    // Remove the enchantment at pos from the history and clear memory
    delete enchantmentHistory.back();
    enchantmentHistory.pop_back();
}

void Minion::printOutEnchantments() {

    // Print out the minion
    for (int i = 0; i < CARD_HEIGHT; i++){
        std::cout << displayCard()[i] << std::endl;
    }
    
    // Get number of enchantments
    int length = enchantmentHistory.size();
    
    // No enchant to print
    if (length == 0) {
        std::cout << "[SORCERY]: No enchantments to display." << std::endl;
        return;
    }

    // Calc number of rows of 5
    int groups = length / ENCHANT_LEN;
    int remainder = length % ENCHANT_LEN;

    std::vector<std::vector<std::string>> output;

    int indexTrace = 0;

    // For every full group, print out the enchantments
    for (int group = 0; group < groups; group++) {

        // Add to vector to be printed
        for (int index = 0; index < ENCHANT_LEN; index++) {
            output.push_back(enchantmentHistory[indexTrace]->displayCard());
            indexTrace++;
        }

        // Print out that row
        for (int a = 0; a < CARD_HEIGHT; ++a){
            for (int j = 0; j < ENCHANT_LEN; j++){
                std::cout << output[j][a];
            }
            std::cout << std::endl;
        }

        // Reset the vector output
        output.clear();
    }

    // Print out remainder to output
    for (int index = 0; index < remainder; index++) {
        output.push_back(enchantmentHistory[indexTrace]->displayCard());
        indexTrace++;
    }

    // Print out the output
    for (int a = 0; a < CARD_HEIGHT; ++a){
        for (int j = 0; j < remainder; j++){
            std::cout << output[j][a];
        }
        std::cout << std::endl;
    }

    // Clear
    output.clear();
}

std::vector<std::string> Minion::displayCard(){

    // Both being nullptr means we have a general minion
    if ((activated == nullptr) && ((triggered == nullptr))) {
        return display_minion_no_ability(getName(), getCost(), getAttack(), getDefence());

    // Activated is nullptr means we have a Triggered Minion
    } else if (activated == nullptr) {
        return display_minion_triggered_ability(getName(), getCost(), getAttack(), 
        getDefence(), triggered->getDescription());
        
    // Then Triggered is nullptr means we have a Activated Minion
    } else {
        return display_minion_activated_ability(getName(), getCost(), getAttack(), 
        getDefence(), activated->getCost(), activated->getDescription());
    }
}

void Minion::useCardOnCard(Player * activePlayer, Player * affectedPlayer, int pos) {

    if (getActivated() != nullptr) {
        // Use the activated ability on the minion at position pos of the affectedPlayer
        getActivated()->useCard(activePlayer, affectedPlayer, pos, this);

    } else {
        // No activated ability
        throw SorceryException("Attempting to use a minion with no activated ability.");
    }
}

void Minion::notify(Subject * whoNotified, State stateOfGame) {

    // Minion died
    if (stateOfGame == State::MinionExitPlay) {
        if (getName() == MIN_B) {
            // Apply bomb Effect
            auto min = static_cast<Minion*>(whoNotified);
            triggered->useCard(min->getOwner(), min->getOwner(), -getAttack());
        }

    // Minion enters play
    } else if (stateOfGame == State::MinionEnterPlay) {
        if (getName() == MIN_FE) {
            // Assply fire elem Effect
            auto min = static_cast<Minion*>(whoNotified);
            triggered->useCard(getOwner(), min->getOwner(), 0);
        }
    
    // End of turn
    } else if (stateOfGame == State::EndOfTurn) {
        if (getName() == MIN_PS) {
            // Apply potion seller Effect
            triggered->useCard(getOwner(), getOwner(), 0);    
        }
    }
}




