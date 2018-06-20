#ifndef LEVEL_H
#define LEVEL_H

#include "list.h"
#include "gamedefs.h"
#include <stdlib.h>
#include "renderinit.h"

void initialiseLevel(game_state* gameState, LEVEL_NUM levelNum);

void reinitialiseCurrLevel(game_state* gameState);

spawn_details* alloc_spawn_detail();

spawn_details* create_spawn_detail(SPAWN_LOC spawnDir, double initialX, double initialY, long numTicksBeforeSpawn);

level_t* create_level_one(void);

void updateScore(level_t* level, HIT_TYPE hitType);

void free_level(level_t* level);


#endif
