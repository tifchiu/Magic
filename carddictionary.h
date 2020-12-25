#ifndef CARDDICT_H
#define CARDDICT_H

#include <map>
#include <string>
#include "minion.h"
#include "enchantment.h"
#include "ritual.h"
#include "spell.h"
#include "abilityactivated.h"
#include "abilitytriggered.h"
#include "state.h"
#include "cardconstants.h"

// Card map for all cards
extern std::map<std::string, Card*> cardDict;

// Ability map to add to potential cards with abilities
extern std::map<std::string, Ability*> abilityDict;

#endif

