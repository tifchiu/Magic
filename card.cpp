#include "card.h"
#include <string>
#include "ascii_graphics.h"
#include "player.h"

// see .h for documentation

Card::Card(std::string name, std::string type, int cost) :
    name{name},
    ownedByPlayer{nullptr},
    cardType{type}, 
    cost{cost}, 
    attack{0}, 
    defence{0} 
{}

Card::Card(std::string name, std::string type, int cost, int atk, int def) :
    name{name},
    ownedByPlayer{nullptr},
    cardType{type}, 
    cost{cost}, 
    attack{atk}, 
    defence{def} 
{}

Card::Card (const Card & other) : 
    name{other.name},
    ownedByPlayer{other.ownedByPlayer},
    cardType{other.cardType},
    cost{other.cost},
    attack{other.attack},
    defence{other.defence}
{}

std::string Card::getName() const {
    return name;
}


void Card::setCardType(std::string ct) {
    cardType = ct;
}

std::string Card::getCardType() const {
    return cardType;
}
  
int Card::getCost() const {
    return cost;
}

void Card::setName(std::string name) {
    this->name = name;
}

void Card::setOwner(Player * owner) {
    ownedByPlayer = owner;
}

Player * Card::getOwner() {
    return ownedByPlayer;
}
void Card::setCost(int cost) {
    cost = cost;
}

int Card::getAttack() const {
    return attack;
}

void Card::setAttack(int atk) {
    attack = atk;
}

int Card::getDefence() const {
    return defence;
}

void Card::setDefence(int def) {
    defence = def;
}

std::string Card::getDescription(){
    return description;
}

void Card::useCard(Player *activePlayer, int pos) {
    return;
}

void Card::useCardOnCard(Player * activePlayer, Player * affectedPlayer, int pos1, int pos2) {
    return;
}

// Returns a vector of strings to print out for the Card class
std::vector<std::string> Card::displayCard() {
    std::vector<std::string> thisIsNeverRan;
    return thisIsNeverRan;
}

