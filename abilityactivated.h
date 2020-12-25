#ifndef ABILITYACTIVATED_H_
#define ABILITYACTIVATED_H_

#include "ability.h"
#include <string>
#include "player.h"

class Minion;

// This is the derived class of an Ability

class AbilityActivated : public Ability {
    int actCost;                          // Activation cost
    std::string abilityDescription;       // Activated ability description
    
public: 

    // AbilityActivated ctor
    AbilityActivated(std::string description, int cost);
    AbilityActivated(const AbilityActivated & other);

    // AbilityActivated dtor
    virtual ~AbilityActivated();

    // Cost getter
    int getCost();
    // Cost setter
    void setCost(int cost);

    // AbilityActivated description getter
    std::string getDescription();

    // useCard(activePlayer, affectedPlayer, int pos) applies the unique activated ability from the activePlayer on the affectedPlayer's card at some position
    void useCard(Player *activePlayer, Player *affectedPlayer, int pos, Minion * attackingMinion);

    friend class Ability;
};

#endif
