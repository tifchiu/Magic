#ifndef RITUAL_H
#define RITUAL_H
#include <iostream>
#include "card.h"
#include "abilitytriggered.h"
#include <vector>
#include "minion.h"
#include "player.h"
#include <memory>
#include "ascii_graphics.h"
#include "state.h"

class Minion;
class Player;

class Ritual : public Card {
    int charges;
    int activationCost;
    AbilityTriggered * triggered;

    public:

    // Ritual constructor
    Ritual(std::string name, std::string type, int cost, int activationCost, int charges, AbilityTriggered * triggered);
    Ritual(Ritual * other);
    
    // Ritual deconstructor
    virtual ~Ritual();

    // get the number of Charges in a Ritual
    int getCharges();

    // get a reference to the Triggered Ability from the Ritual Card
    AbilityTriggered & getActualTriggered();
    
    // get a pointer to the Triggered Ability from the Ritual Card
    AbilityTriggered * getTriggered();

    // set the Triggered Ability
    void setTriggered(AbilityTriggered * ptr);

    // set the Charges on the Ritual Card
    void setCharges(int charges);
    // get the Activation Cost of the Ritual Card
    int getACost();
    // set the Activation Cost of the Ritual Card
    void setACost(int aCost);

    // use a Ritual Card on activePlayer's pos-th position minion
    void useCard(Player *activePlayer, int pos) override;
    
    // display card function for rituals
    std::vector<std::string> displayCard() override;

};
#endif
