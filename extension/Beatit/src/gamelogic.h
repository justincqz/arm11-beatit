#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "gamedefs.h"

game_state* initialiseGameState();

void update(game_state* gameState);

void printState(game_state* gameState);

void free_game_state(game_state* gameState);

#endif
