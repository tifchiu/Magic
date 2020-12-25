#include "carddictionary.h"

// This is the complete *dictionary* of every card in the game

std::map<std::string, Card*> cardDict = {

{MIN_AE, new Minion{MIN_AE, TYPE_M, 0, 1, 1}},
{MIN_EE, new Minion{MIN_EE, TYPE_M, 3, 4, 4}},
{MIN_B, new Minion{MIN_B, TYPE_M, 2, 1, 2, new AbilityTriggered{INFO_B, State::MinionExitPlay}}},
{MIN_FE, new Minion{MIN_FE, TYPE_M, 2, 2, 2, new AbilityTriggered{INFO_FE, State::MinionEnterPlay}}},
{MIN_PS, new Minion{MIN_PS, TYPE_M, 2, 1, 3, new AbilityTriggered{INFO_PS, State::EndOfTurn}}},
{MIN_NP, new Minion{MIN_NP, TYPE_M, 1, 0, 1, new AbilityActivated{INFO_NP, 1}}},
{MIN_AS, new Minion{MIN_AS, TYPE_M, 1, 1, 1, new AbilityActivated{INFO_AS, 1}}},
{MIN_MS, new Minion{MIN_MS, TYPE_M, 3, 2, 3, new AbilityActivated{INFO_MS, 2}}},

{SP_BAN, new Spell{SP_BAN, TYPE_S, INFO_BAN, 2, new AbilityActivated{INFO_BAN, 100}}},
{SP_UNSUM, new Spell{SP_UNSUM, TYPE_S, INFO_UNSUM, 1, new AbilityActivated{INFO_UNSUM, 100}}},
{SP_RECH, new Spell{SP_RECH, TYPE_S, INFO_RECH, 1, new AbilityActivated{INFO_RECH, 100}}},
{SP_DISEN, new Spell{SP_DISEN, TYPE_S, INFO_DISEN, 1, new AbilityActivated{INFO_DISEN, 100}}},
{SP_RAIDEA, new Spell{SP_RAIDEA, TYPE_S, INFO_RAIDEA, 1, new AbilityActivated{INFO_RAIDEA, 100}}},
{SP_BLIZ, new Spell{SP_BLIZ, TYPE_S, INFO_BLIZ, 3, new AbilityActivated{INFO_BLIZ, 100}}},

{EN_GIASTR, new Enchantment{EN_GIASTR, TYPE_E, INFO_GIASTR, 1, '+', 2, '+', 2}},
{EN_ENR, new Enchantment{EN_ENR, TYPE_E, INFO_ENR, 2, '*', 2, '-', 2}},
{EN_DEL, new Enchantment{EN_DEL, TYPE_E, INFO_DEL, 1}},
{EN_MAGFAT, new Enchantment{EN_MAGFAT, TYPE_E, INFO_MAGFAT, 0}},
{EN_SILEN, new Enchantment{EN_SILEN, TYPE_E, INFO_SILEN, 1}},

{RIT_DARRIT, new Ritual{RIT_DARRIT, TYPE_R, 0, 1, 5, new AbilityTriggered{INFO_DARRIT, State::StartOfTurn}}},
{RIT_AUROPOW, new Ritual{RIT_AUROPOW, TYPE_R, 1, 1, 4, new AbilityTriggered{INFO_AUROPOW, State::MinionEnterPlay}}},
{RIT_STAND, new Ritual{RIT_STAND, TYPE_R, 3, 2, 4, new AbilityTriggered{INFO_STAND, State::MinionEnterPlay}}}
};




