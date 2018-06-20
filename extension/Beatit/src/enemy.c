#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include "gfx.h"
#include "list.h"
#include <limits.h>
#include "render.h"
#include "player.h"
#include "enemy.h"
#include "gamelogic.h"
#include "gamedefs.h"
#include "level.h"

enemy_type* spawnEnemy(SPAWN_LOC spawnDir, double initialX, double initialY) {
  // printf("Spawned enemy...\n");
  enemy_type* enemy = malloc(sizeof(enemy_type));

  enemy->x = initialX;
  enemy->y = initialY;
  enemy->range = 50;
  enemy->status = ALIVE;
  enemy->action = MOVE;

  enemy->cooldowns = malloc(sizeof(enemy_cooldowns));
  enemy->cooldowns->deathCounter = 10;
  enemy->cooldowns->attackCounter = 0;

  enemy->speed = malloc(sizeof(speed_type));
  enemy->speed->dy = 0;
  if (spawnDir == RIGHT_OF_MC) {
    enemy->speed->dx = -1 * ENEMY_MOVE_SPEED;
  } else {
    enemy->speed->dx = ENEMY_MOVE_SPEED;
  }

  return enemy;
}

void free_enemy(enemy_type* enemy) {
  // free(enemy->speed);
  // //
  // // //TO-DO FREE RENDER
  // //
  // // free(enemy->cooldowns);
  free(enemy);
}

void deleteEnemy(game_state* gameState, enemy_type* enemy) {
  gameState->level->numEnemies--;
  list_remove(gameState->enemies, enemy);
  free_enemy(enemy);
  printf("Enemy deleted..\n");
}

void enemyDie(game_state* gameState, enemy_type* enemy) {
  if (enemy->cooldowns->deathCounter == 0) {
    deleteEnemy(gameState, enemy);
  }
  enemy->cooldowns->deathCounter--;
}

void receiveAttack(game_state* gameState, enemy_type* enemy) {
  // Justin: Code to render hitbox
  if (enemy->beat->currentSt != NULL) {
    enemy->beat->currentSt = NULL;
  }

  enemy->x += enemy->speed->dx;
  enemy->y += enemy->speed->dy;
  enemy->status = DEAD;

  if (enemy->y < FLOOR_Y) {
    enemy->speed->dy += 20.81;
  } else {
    enemy->speed->dy = 0;
    enemy->y = FLOOR_Y;
    printf("enemy set die\n");
    enemy->action = DIE;
  }
}

void enemyReady(game_state* gameState, enemy_type* enemy) {
  if (fabs(gameState->player->x - enemy->x) < enemy->range) {
    // printf("Enemy in range to attack player! Attacking player...\n");
    enemy->action = ATTACK;
    return;
  }

  enemy->x += enemy->speed->dx;
}

void moveEnemy(game_state* gameState, enemy_type* enemy) {
  if (fabs(gameState->player->x - enemy->x) < gameState->player->range) {
    if (enemy->speed->dx < 0) {
      list_insert(gameState->rightHitBox, enemy);
    } else {
      list_insert(gameState->leftHitBox, enemy);
    }

    enemy->action = GETTING_READY;
    return;
  }

  // printf("Enemy moved from %f to ", enemy->x);
  enemy->x += enemy->speed->dx;
  // printf("%f.\n", enemy->x);
}

void enemyAttack(game_state* gameState, enemy_type* enemy) {
  if (gameState->player->lives > 0) {
    gameState->player->lives--;
    gameState->level->combo = 0;
    updateScore(gameState->level, MISS);
  }

  if (enemy->speed->dx < 0) {
    list_remove(gameState->rightHitBox, enemy);
  } else {
    list_remove(gameState->leftHitBox, enemy);
  }

  deleteEnemy(gameState, enemy);
}

static void updateCooldowns(enemy_type* enemy) {
  enemy->cooldowns->attackCounter = enemy->cooldowns->attackCounter == 0 ? 0 : enemy->cooldowns->attackCounter - 1;
}

void updateEnemy(game_state* gameState, enemy_type* enemy) {
  switch (enemy->action) {
    case GETTING_READY:
      enemyReady(gameState, enemy);
      break;
    case MOVE:
      moveEnemy(gameState, enemy);
      break;
    case ATTACK:
      enemyAttack(gameState, enemy);
      break;
    case RECEIVING_ATTACK:
      receiveAttack(gameState, enemy);
      break;
    case DIE:
      enemyDie(gameState, enemy);
      break;
  }

  updateCooldowns(enemy);
}
