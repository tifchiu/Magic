#include "ability.h"

// see .h for documentation

Ability::Ability(std::string description) : 
    description(description)
{}

Ability::Ability(const Ability & other) :
    description{other.description}
{}

std::string Ability::getDescription() {
    return description;
}

void Ability::setDescription(std::string description){
    this->description = description;
}
