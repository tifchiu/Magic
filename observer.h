#ifndef _OBSERVER_H
#define _OBSERVER_H
#include "state.h"

class Subject;

// This is the Observer class

class Observer {
  public:
  // Observers can notify
  virtual void notify(Subject * whoNotified, State stateOfGame) = 0;

  // Dtor
  virtual ~Observer() = default;
};

#endif
