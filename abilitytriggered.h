#ifndef ABILITYTRIGGERED_H
#define ABILITYTRIGGERED_H

#include <string>

#include "ability.h"
#include "state.h"
#include "player.h"

// AbilityTriggered class is a derived class of Ability

class AbilityTriggered : public Ability {

    State condition;                            // Game condition

    std::string abilityDescription;             // AbilityTriggered description

    public:

    // Ctor
    AbilityTriggered(std::string description, State condition);
    AbilityTriggered(const AbilityTriggered & other);
    // Dtor
    virtual ~AbilityTriggered() = default;
    
    // Condition getter
    State getCondition();
    // Condition setter
    void setCondition(State condition);

    // useCard(activePlayer, affectedPlayer, pos) applies a triggered ability 
    //    from activePlayer to the affectedPlayer's card at position pos
    void useCard(Player *activePlayer, Player *affectedPlayer, int pos);

    friend class Ability;

};

#endif

