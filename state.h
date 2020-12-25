#ifndef _STATE_H
#define _STATE_H

// Enumerated class describing specific game states

enum class State {
  GameStart,
  GameEnd,
  EndOfTurn,
  StartOfTurn,
  MinionEnterPlay,
  MinionExitPlay
};

#endif
