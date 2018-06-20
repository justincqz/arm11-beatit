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

player_type* initialisePlayer(double initialX, double initialY) {
  player_type* player = malloc(sizeof(player_type));
  player->x = initialX;
  player->y = initialY;
  player->range = 100;
  player->render = initializeMCRender(SCREEN_WIDTH/ 2, SCREEN_HEIGHT /2);

  player->cooldowns = malloc(sizeof(player_cooldowns));
  player->cooldowns->attackCounter = 0;
  player->cooldowns->deathCounter = 10;

  return player;
}

int attackLeft(game_state* gameState) {

  if (gameState->player->cooldowns->attackCounter != 0) {
    // printf("attack on cooldown!\n");
    return 0;
  }

  list_elem* curr_elem = list_get_first(gameState->leftHitBox);

  if (curr_elem != NULL) {
    enemy_type* enemyToHit = (enemy_type*) curr_elem->value;
    list_remove(gameState->leftHitBox, enemyToHit);
    enemyToHit->status = DEAD;
    enemyToHit->action = RECEIVING_ATTACK;
    enemyToHit->speed->dx = -1 * ENEMY_FLY_SPEED_DX;
    enemyToHit->speed->dy = -1 * ENEMY_FLY_SPEED_DY;

    updateScore(gameState->level, HIT);
    gameState->level->combo++;
  } else {
    gameState->player->cooldowns->attackCounter = PLAYER_ATTACK_COOLDOWN;
  }

  return 1;
}

int attackRight(game_state* gameState) {

  if (gameState->player->cooldowns->attackCounter != 0) {
    // printf("attack on cooldown!\n");
    return 0;
  }

  list_elem* curr_elem = list_get_first(gameState->rightHitBox);

  if (curr_elem != NULL) {
    enemy_type* enemyToHit = (enemy_type*) curr_elem->value;
    list_remove(gameState->rightHitBox, enemyToHit);
    enemyToHit->status = DEAD;
    enemyToHit->action = RECEIVING_ATTACK;
    enemyToHit->speed->dx = ENEMY_FLY_SPEED_DX;
    enemyToHit->speed->dy = -1 * ENEMY_FLY_SPEED_DY;

    updateScore(gameState->level, HIT);
    gameState->level->combo++;
  } else {
    gameState->player->cooldowns->attackCounter = PLAYER_ATTACK_COOLDOWN;
  }

  return 1;

}

static void updateCooldowns(player_type* player) {
  if (player->cooldowns->attackCounter != 0) {
    // printf("attack on cooldown!\n");
  }

  player->cooldowns->attackCounter = player->cooldowns->attackCounter == 0 ? 0 : player->cooldowns->attackCounter - 1;
}

void updatePlayer(player_type* player) {
  // if (player->status != DEAD) {
  //   updateCooldowns(player);
  // } else {
  //   printf("player already dead!\n");
  // }
  updateCooldowns(player);
}

void free_player(player_type* player) {
  free_render(player->render);
  free(player->cooldowns);
  free(player);
}
