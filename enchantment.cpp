#include "enchantment.h"
#include "ascii_graphics.h"

Enchantment::Enchantment(std::string name, std::string cardType, std::string description, int cost) : 
    Card{name, cardType, cost}, 
    description{description},
    operationAtk{' '},
    operationDef{' '}
{}

Enchantment::Enchantment(std::string name, std::string cardType, 
        std::string description, int cost, char opAtk, int atk, char opDef, int def) :
    Card{name, cardType, cost, atk, def}, 
    description{description},
    operationAtk{opAtk},
    operationDef{opDef}
    
{}

Enchantment::Enchantment(Enchantment * other) :
    Card(other->getName(), other->getCardType(), other->getCost(), other->getAttack(), other->getDefence()),
    description{other->getDescription()},
    operationAtk{other->getOperationAtk()},
    operationDef{other->getOperationDef()}

{}

Enchantment::~Enchantment(){
    
}

std::string Enchantment::getDescription(){
    return description;
}

char Enchantment::getOperationAtk() {
    return operationAtk;
}

char Enchantment::getOperationDef(){
    return operationDef;
}

std::vector<std::string> Enchantment::displayCard(){
    // Any enchantment that has empty description is a numerical enchantment
    if (getDescription() == "") {

        // Set up output as string
        std::string attackField = std::string(1, getOperationAtk()) + std::to_string(getAttack());
        std::string defenceField = std::string(1, getOperationDef()) + std::to_string(getDefence());

        // Create card with atk and def
        return display_enchantment_attack_defence(getName(), getCost(), description, attackField, defenceField);
    } else {
        
        // Enchantments that have descriptions but no attack and defence fields
        return display_enchantment(getName(), getCost(), description);
    }
}
