#include <algorithm>
#include "subject.h"
#include "minion.h"

void Subject::attach(Observer *o) { observers.emplace_back(o); }

void Subject::detach(Observer *o) {
    auto x = std::find(observers.begin(), observers.end(), o);
    observers.erase(x);
}

void Subject::notifyObservers(State stateOfGame) {
    auto thisMin = static_cast<Minion*>(this);
    
    // Find first observer of other player
    for (auto &ob : observers) {
        auto observerMin = static_cast<Minion*>(ob);

        // Get first instance
        if (thisMin->getOwner() != observerMin->getOwner() ) {
            thisMin->notify( observerMin, stateOfGame );
            break;
        }
        
    }
}

int Subject::numOfObservers() {
    return observers.size();
}
