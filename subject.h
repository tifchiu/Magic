#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "state.h"
#include "observer.h"

class Observer;

// This is the subject classes u

class Subject {

    std::vector<Observer*> observers;   // Set of observers

    public:

    // Add new o to observers
    void attach(Observer *o);

    // Remove o from the observers
    void detach(Observer *o);

    // Notify observers based on state of game for the other player
    void notifyObservers(State);

    // get num of observers
    int numOfObservers();
};

#endif
