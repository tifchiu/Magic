#include "abilityactivated.h"
#include "cardconstants.h"
#include "carddictionary.h"
#include "sorceryException.h"


// See .h for more documentation

AbilityActivated::AbilityActivated(std::string description, int cost):
    Ability(description), 
    abilityDescription{description},
    actCost{cost} 
{}

AbilityActivated::~AbilityActivated() {}

AbilityActivated::AbilityActivated(const AbilityActivated & other) :
    Ability(other),
    abilityDescription{other.abilityDescription},
    actCost{other.actCost}
{}

int AbilityActivated::getCost(){
    return this->actCost;
}

void AbilityActivated::setCost(int cost){
    actCost = cost;
}

std::string AbilityActivated::getDescription() {
    return abilityDescription;
}

// Extra notes:
// useCard for activated abilities are called to account for the effect of 
// activated abilities of Minions.
// Activated abilities are used when the minion is already on the board
// and is used (eg. use i).

void AbilityActivated::useCard(Player *activePlayer, Player *affectedPlayer, int pos, Minion * attackingMinion) {
    // Get information about the affected minion
    int boardSize = activePlayer->getBoardSize();
    int actionPoints = attackingMinion->getActionPoints();
    bool isSilenced = attackingMinion->getSilenced();

    // Not enough action points
    if (actionPoints < 1) {
        std::cout << "[Sorcery]: Minion has no action points." << std::endl;
        return;
    
    // Minion is silenced
    } else if (isSilenced) {
        std::cout << "[Sorcery]: Minion is silenced." << std::endl;
        return;
    }

    // Player current magic
    int curMagic = activePlayer->getMagic();

    // Enough magic, decrease magic by cost
    if (curMagic >= actCost) {
        activePlayer->setMagic(curMagic - actCost);
    
    // Not enough magic
    } else {
        std::cout << "[Sorcery]: Not enough magic for this action." << std::endl;
        return;
    }

    // Set action points to 0
    attackingMinion->setActionPoints(0);
    
    // Run the activated ability

    std::string cardDes = getDescription();

    
    // Novice pyromancer
    if (cardDes == INFO_NP) {

        // Deal 1 damage to target minion
        bool isAlive = affectedPlayer->getElemFromBoard(pos)->changeHealthPoints(-1);
        if (!isAlive) {
            affectedPlayer->removeFromBoardToGraveYard(pos);
        }
    
    // Apprentice summoner
    } else if (cardDes == INFO_AS){

        // Board is full
        if (boardSize >= FULL_BOARD){
            std::cout << "[Sorcery]: Wasted magic :( There is no space for anything to summon!" << std::endl;
            return;
        } else {
                // create new Air elemental and set is owner to the active player
            Card* newAirElemental = new Minion(static_cast<Minion*>(cardDict[MIN_AE]));
            newAirElemental->setOwner(activePlayer);

                // attach observer relationships. add card to board
            activePlayer->attachObserversToNewMinion(newAirElemental);

            activePlayer->addCardToBoard(newAirElemental);

            activePlayer->attachNewMinionToObservers(newAirElemental);

            affectedPlayer->attachObserversToNewMinion(newAirElemental);

            affectedPlayer->attachNewMinionToObservers(newAirElemental);

            if (activePlayer->hasRitual() && activePlayer->getRitual()->getName() == RIT_STAND) {
                try {
                    activePlayer->getRitual()->useCard(activePlayer, 0);
                } catch (SorceryException & e) {

                }
            } else if (affectedPlayer->hasRitual() && affectedPlayer->getRitual()->getName() == RIT_STAND) {
                try {
                    affectedPlayer->getRitual()->useCard(affectedPlayer, 0);
                } catch (SorceryException & e) {

                }
            } else {
                auto whoNotified = static_cast<Minion*>(newAirElemental);
                affectedPlayer->notifyMinions(whoNotified, State::MinionEnterPlay);
            } 
        }

    // Master summoner
    } else if (cardDes == INFO_MS) {
        // Summon up to three 1/1 air elementals
        const int numToSum = 3;

        boardSize = activePlayer->getBoardSize();

        // Calculate open slots to use
        if (boardSize < numToSum) {
            boardSize = numToSum;
        } else {
            boardSize = FULL_BOARD - boardSize;
        }

        // For every slot open out of 5, we add a new M_AE up to 3 times
        for (int times = 0 ; times < boardSize; times++) {

            // for up to 3 times we add the minion, and promptly destroy it if need be

            Card * newAE = new Minion(static_cast<Minion*>(cardDict[MIN_AE]));
            newAE->setOwner(activePlayer);

            activePlayer->attachObserversToNewMinion(newAE);

            activePlayer->addCardToBoard(newAE);

            activePlayer->attachNewMinionToObservers(newAE);
            affectedPlayer->attachObserversToNewMinion(newAE);
            affectedPlayer->attachNewMinionToObservers(newAE);
            
            if (activePlayer->hasRitual() && activePlayer->getRitual()->getName() == RIT_STAND) {
                try {
                    activePlayer->getRitual()->useCard(activePlayer, 0);
                } catch (SorceryException & e) {

                }
            } else if (affectedPlayer->hasRitual() && affectedPlayer->getRitual()->getName() == RIT_STAND) {
                try {
                    affectedPlayer->getRitual()->useCard(activePlayer, 0);
                } catch (SorceryException & e) {
                    
                }
            } else {
                auto whoNotified = static_cast<Minion*>(newAE);
                affectedPlayer->notifyMinions(whoNotified, State::MinionEnterPlay);
            }   
        }
    }
}
