#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
#include <vector>

// This is the Card class.
// It is the base class of Enchantment, Minion, Spell, and Ritual

class Player;

class Card {

    std::string name;           // Name of the card
    std::string cardType;       // Card type
    int cost;                   // Cost to play card
    std::string description;    // Card description
    int attack;                 // Card attack power
    int defence;                // Card defense 
    Player * ownedByPlayer;     // Player owner of card
    
    public:
  
    // Ctor with a name, type, and cost
    Card(std::string name, std::string type, int cost);

    // Ctor with a name, type, cost, attack, and defence
    Card(std::string name, std::string type, int cost, int atk, int def);

    // Copy ctor
    Card (const Card & other);
    
    // Dtor
    virtual ~Card() = default;

    // Name getter
    std::string getName() const;
    // Name setter
    void setName(std::string name);

    // Owner getter
    Player * getOwner(); 
    // Owner setter
    void setOwner(Player * owner);

    // Card type getter
    std::string getCardType() const;
    // Card type setter
    void setCardType(std::string ct);

    // Cost getter
    int getCost() const;
    // Cost setter
    void setCost(int cost);

    // Attack getter
    int getAttack() const;
    // Attack setter
    void setAttack(int atk);
    
    // Defence getter
    int getDefence() const;
    // Defence setter
    void setDefence(int def);

    // Description getter
    std::string getDescription();

    // Overloaded functions by derived classes
    // Not used by Card class
    virtual void useCard(Player *activePlayer, int pos);
    virtual void useCardOnCard(Player * activePlayer, Player * affectedPlayer, int pos1, int pos2);
    
    // Returns a vector of strings to print out for the Card class
    virtual std::vector<std::string> displayCard();

};

#endif


