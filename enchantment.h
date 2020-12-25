#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H
#include <iostream>
#include <memory>
#include <vector>
#include "card.h"

// An Enchantment is a derived class of Card
// It gives Minions special effects

class Enchantment : public Card {

    std::string description;    // Enchantment description

    char operationAtk;          // Potential math character for attack
    char operationDef;          // Potential math character for defence

    public:

    // Ctor with a name, card type, description, and cost
    Enchantment(std::string name, std::string cardType, std::string description, int cost);

    // Copy Ctor
    Enchantment(Enchantment * other);

    // Dtor
    virtual ~Enchantment();
    
    // Whole Enchantment Ctor
    Enchantment(std::string name, std::string cardType, std::string description,  
    int cost, char opAtk, int atk, char opDef, int def);
    
    // description getter
    std::string getDescription();

    // Enchantment math getters
    char getOperationAtk();
    char getOperationDef();

    // Return vector of strings to be printed out for a Enchantment card
    std::vector<std::string> displayCard();
};

#endif

