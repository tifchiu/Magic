#include "abilitytriggered.h"
#include "cardconstants.h"

AbilityTriggered::AbilityTriggered(std::string description, State condition) :
    Ability(description),
    abilityDescription{description},
    condition{condition}
{}

AbilityTriggered::AbilityTriggered(const AbilityTriggered & other) :
    Ability(other),
    abilityDescription{other.abilityDescription},
    condition{other.condition}
{}

void AbilityTriggered::setCondition(State condition){
    this->condition = condition;
}

State AbilityTriggered::getCondition(){
    return condition;
}

// useCard for triggered abilities is used given that the game State condition
// on the card is satsified. 
void AbilityTriggered::useCard(Player *activePlayer, Player *affectedPlayer, int pos){

    int boardSizeAff = affectedPlayer->getBoardSize();

    // Pos is the position of the target
    // abilities of Minions 
    std::string cardDes = abilityDescription;

    // Bomb
    if (cardDes == INFO_B){
        // Deals damage to all the opponent minions equal to its attack value when it dies
        // If this effect occurs, assume that position is the attack value
        affectedPlayer->damageMinionsOnBoard(activePlayer, affectedPlayer, pos);

    // Fire Elemental
    } else if (cardDes == INFO_FE){
        // Whenever an opponent's minion enters play, deal 1 damage to it
        bool isAlive = affectedPlayer->getElemFromBoard(boardSizeAff)->changeHealthPoints(-1);
        if (!isAlive) {
            affectedPlayer->removeFromBoardToGraveYard(boardSizeAff);
        }
    
    // Potion Seller
    } else if (cardDes == INFO_PS){
        // "At the end of your turn, all your minions gain +0/+1"
        activePlayer->healMinionsOnBoard(1);
     }
}
