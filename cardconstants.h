#ifndef CARDCONST_H
#define CARDCONST_H

#include <string>

// This is a file to store all constants needed in the game

const std::string MIN_AE = "Air Elemental";
const std::string MIN_EE = "Earth Elemental";
const std::string MIN_B = "Bomb";
const std::string MIN_FE = "Fire Elemental";
const std::string MIN_PS = "Potion Seller";
const std::string MIN_NP = "Novice Pyromancer";
const std::string MIN_AS = "Apprentice Summoner";
const std::string MIN_MS = "Master Summoner";
const std::string SP_BAN = "Banish";
const std::string SP_UNSUM = "Unsummon";
const std::string SP_RECH = "Recharge";
const std::string SP_DISEN = "Disenchant"; 
const std::string SP_RAIDEA = "Raise Dead"; 
const std::string SP_BLIZ = "Blizzard";
const std::string EN_GIASTR = "Giant Strength";
const std::string EN_ENR = "Enrage"; 
const std::string EN_DEL = "Delay";
const std::string EN_MAGFAT = "Magic Fatigue"; 
const std::string EN_SILEN = "Silence"; 
const std::string RIT_DARRIT = "Dark Ritual"; 
const std::string RIT_AUROPOW = "Aura of Power"; 
const std::string RIT_STAND = "Standstill";

const std::vector<std::string> cardsWithTarget = {"Disenchant", "Unsummon", "Blizzard", "Banish"};

const std::vector<std::string> allCards = {"Air Elemental","Earth Elemental","Bomb","Fire Elemental","Potion Seller","Novice Pyromancer","Apprentice Summoner","Master Summoner","Banish","Unsummon","Recharge","Disenchant" ,"Raise Dead" ,"Blizzard","Giant Strength","Enrage" ,"Delay","Magic Fatigue" ,"Silence" ,"Dark Ritual" ,"Aura of Power" ,"Standstill"};


const std::vector<std::string> cardsWithTrigger = {"Bomb", "Fire Elemental", "Potion Seller"};

const std::vector<std::string> cardsWithActivated = {"Novice Pyromancer", "Apprentice Summoner", "Master Summoner"};


const std::string TYPE_M = "Minion";
const std::string TYPE_S = "Spell";
const std::string TYPE_E = "Enchantment";
const std::string TYPE_R = "Ritual";

const std::string INFO_B = 
"Deals damage to all the opponent minions equal to its attack value when it dies";
const std::string INFO_FE = "Whenever an opponent's minion enters play, deal 1 damage to it";
const std::string INFO_PS = "At the end of your turn, all your minions gain +0/+1";
const std::string INFO_NP = "Deal 1 damage to target minion";
const std::string INFO_AS = "Summon a 1/1 air elemental";
const std::string INFO_MS = "Summon up to three 1/1 air elementals";
const std::string INFO_BAN = "Destroy target minion or ritual";

const std::string INFO_UNSUM = "Return target minion to its owner's hand";
const std::string INFO_RECH = "Your ritual gains 3 charges";
const std::string INFO_DISEN = "Destroy the top enchantment on target minion"; 
const std::string INFO_RAIDEA = 
"Resurrect the top minion in your graveyard and set its defence to 1"; 
const std::string INFO_BLIZ = "Deal 2 damage to all minions";

const std::string INFO_GIASTR = "";
const std::string INFO_ENR = ""; 
const std::string INFO_DEL = 
"Enchanted minion does not gain an action on their next turn. This enchantment is automatically destroyed after 1 turn";
const std::string INFO_MAGFAT = "Enchanted Minion's activated ability costs 2 more"; 
const std::string INFO_SILEN = "Enchanted minion cannot use abilities"; 
const std::string INFO_DARRIT = "At the start of your turn, gain 1 magic"; 
const std::string INFO_AUROPOW = 
"Whenever a minion enters play under your control, it gains +1/+1"; 
const std::string INFO_STAND = "Whenever a minion enters play, destroy it";

#endif
