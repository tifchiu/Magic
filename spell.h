#ifndef SPELL_H
#define SPELL_H
#include <iostream>
#include <vector>
#include "card.h"
#include "abilityactivated.h"
#include "minion.h"
#include "cardconstants.h"

class Minion;
class Player;

class Spell : public Card {

    AbilityActivated* activated;

    char operationAttack;
    char operationDefence;

    public:

    // Spell constructor 
    Spell(std::string name, std::string type, std::string description, int cost, AbilityActivated * activated);
    
    // Deconstructor
    virtual ~Spell();
    Spell(Spell * other);
    // get activated ability from spell
    AbilityActivated * getActivated();
    AbilityActivated & getActualActivated();


    // set the Activated Ability for a Spell
    void setActivated(AbilityActivated * ptr);

    // use Spell Card on a target card
    void useCardOnCard(Player *activePlayer, Player *affectedPlayer, int pos1, int pos2) override;

    // use Spell Card (no target)
    void useCard(Player *activePlayer, int pos) override;

    // displays a Spell Card
    std::vector<std::string> displayCard() override;
};

#endif
