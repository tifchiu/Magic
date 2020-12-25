#ifndef ABILITY_H_
#define ABILITY_H_

#include <string>

// This is the Ability base class
// It's derived classes are: AbilityActivated, AbilityTriggered

class Ability {

  std::string description;          // This is an Ability description

public:

    // Primary ctor
    Ability(std::string description);
    Ability(const Ability & other);
    // Dtor
    virtual ~Ability() = default;

    // Getter of description
    std::string getDescription();
    // Setter of description
    void setDescription(std::string description);

};

#endif
