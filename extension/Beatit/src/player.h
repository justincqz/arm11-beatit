#ifndef PLAYER_H
#define PLAYER_H

#include "gamedefs.h"
#include "renderinit.h"

player_type* initialisePlayer(double initialX, double initialY);

int attackLeft(game_state* gameState);

int attackRight(game_state* gameState);

void updatePlayer(player_type* player);

void free_player(player_type* player);

#endif
