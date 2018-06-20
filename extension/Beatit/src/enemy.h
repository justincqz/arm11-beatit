#ifndef ENEMY_H
#define ENEMY_H

#include "gamedefs.h"

enemy_type* spawnEnemy(SPAWN_LOC spawnDir, double initialX, double initialY);

void deleteEnemy(game_state* gameState, enemy_type* enemy);

void receiveAttack(game_state* gameState, enemy_type* enemy);

void enemyReady(game_state* gameState, enemy_type* enemy);

void moveEnemy(game_state* gameState, enemy_type* enemy);

void enemyAttack(game_state* gameState, enemy_type* enemy);

void updateEnemy(game_state* gameState, enemy_type* enemy);

void enemyDie(game_state* gameState, enemy_type* enemy);

#endif
