#ifndef MINION_H
#define MINION_H
#include "card.h"
#include "abilityactivated.h"
#include "abilitytriggered.h"
#include "enchantment.h"
#include "player.h"
#include "observer.h"
#include "subject.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Constants
const int CARD_HEIGHT = 11;
const int ENCHANT_LEN = 5;

class Player;
class AbilityActivated;
class AbilityTriggered;

// The Minion class is a derived class of Card
// Minions are the main characters in the game aside from the players.
// Minons observe eachother

class Minion : public Card, public Subject, public Observer {

    // Minion is under silenced effect
    bool silenced;                    

    // Minion action points, which are used for attacking or abilities
    int actionPoints;

    // Minion's special abilities - either activated or triggered
    AbilityActivated * activated;
    AbilityTriggered * triggered;

    // Minoion enchantment history contains all of its enchantments as ptrs 
    std::vector<Enchantment *> enchantmentHistory;

    public:

    
    // Minion ctors
    Minion(std::string name, std::string cardType, int cost, int attack, int defence, AbilityActivated * activated, AbilityTriggered * triggered);

    Minion(std::string name, std::string cardType, int cost, int attack, int defence, AbilityActivated * activated);

    Minion(std::string name, std::string cardType, int cost, int attack, int defence, AbilityTriggered * triggered);

    Minion(std::string name, std::string cardType, int cost, int attack, int defence);

    Minion(Minion * other);


    // Dtor of a minion
    virtual ~Minion();

    // Dtor of enchantment history
    void clearEnchantments();

    // AbilityActivated and AbilityTriggered getters as reference
    AbilityActivated & getActualActivated();
    AbilityTriggered & getActualTriggered();

    // AbilityActivated and AbilityTriggered setters
    void setTriggered(AbilityTriggered * ptr);
    void setActivated(AbilityActivated * ptr);

    // AbilityActivated and AbilityTriggered setters as ptr
    AbilityActivated * getActivated();
    AbilityTriggered * getTriggered();


    // This minion will attack the player
    void attackPlayer(Player * player);

    // This minion will attack the opponent's minion at index minionToAttack
    // Returns true if the minion dies from this action
    bool attackMinion(Player * opponent, int minionToAttack);

    // Changes health of this minion by newPts exactly, +/-
    bool changeHealthPoints(int newPts);

    // Returns the number of action points this minion has
    int getActionPoints();
    // Sets the number of action points of this minion
    void setActionPoints(int ap);

    // Returns the state of the minion if it is silenced
    bool getSilenced();

    // This will print out all the enchantments of this minion
    void printOutEnchantments();

    // Returns the size of he enchantmentHistory vector
    int getEnchantmentsHistorySize();

    // Adds an enchantment to enchantmentHistory from the p player's hand at index i on the target
    bool addEnchantment(int i, Player * p, int target);

    // Removes the top enchantment from enchantmentHistory and undo the enchantment
    void removeEnchantment();

    // Gives a vector of strings to print out the Minion
    std::vector<std::string> displayCard() override;

    // active player on the affectedPlayer's minion at index pos
    void useCardOnCard(Player * activePlayer, Player * affectedPlayer, int pos);

    // Observer function to notify based on the state of the game
    void notify (Subject * whoNotified, State stateOfGame) override;

    // Observer function to tell observers if this minion has chnaged state
    void broadcastIfChangedState();

};

#endif
