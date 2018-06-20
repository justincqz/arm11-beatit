#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include "gfx.h"
#include "list.h"
#include <limits.h>
#include "render.h"
#include "renderdef.h"
#include "renderinit.h"
#include "enemy.h"
#include "player.h"
#include "gamelogic.h"
#include "gamedefs.h"
#include "level.h"
#include "menu.h"
#include <stdlib.h>

#define GAME_OVER_DELAY 30

game_state* initialiseGameState() {
  game_state* gameState = malloc(sizeof(game_state));
  gameState->player = initialisePlayer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  gameState->enemies = create_list();
  gameState->leftHitBox = create_list();
  gameState->rightHitBox = create_list();
  gameState->level = NULL;
  gameState->stage = MAIN_MENU;
  gameState->menuState = initialiseMenuState();
  return gameState;
}

static void updateLevel(game_state* gameState) {
  // printf("ticksElapsed: %ld\n", gameState->level->ticksElapsed);

  if (gameState->level->numEnemies == 0) {
    gameState->level->gameOverTick = gameState->level->ticksElapsed;
    gameState->stage = GAME_OVER;
  }

  list_elem* first_elem = list_get_first(gameState->level->spawns);

  if (first_elem != NULL) {
    spawn_details* nextSpawn = (spawn_details*) first_elem->value;

    // printf("nextSpawn at: %ld\n", nextSpawn->numTicksBeforeSpawn);
    if (nextSpawn != NULL && gameState->level->ticksElapsed >= nextSpawn->numTicksBeforeSpawn) {
      enemy_type* newEnemy = spawnEnemy(nextSpawn->spawnDir, nextSpawn->initialX, nextSpawn->initialY);

      if (nextSpawn->spawnDir == LEFT_OF_MC) {
        newEnemy->render = initializeEnemyRender(newEnemy->x, newEnemy->y, RIGHT);
        newEnemy->beat = init_beat_render(newEnemy->x, newEnemy->y, RIGHT);
      } else {
        newEnemy->render = initializeEnemyRender(newEnemy->x, newEnemy->y, LEFT);
        newEnemy->beat = init_beat_render(newEnemy->x, newEnemy->y, LEFT);
      }

      list_insert(gameState->enemies, newEnemy);
      list_remove(gameState->level->spawns, nextSpawn);
    }
  }

  updatePlayer(gameState->player);

  list_elem* curr_elem = list_get_first(gameState->enemies);
  while (curr_elem != NULL) {
    updateEnemy(gameState, (enemy_type*) curr_elem->value);
    curr_elem = list_get_next(curr_elem);
  }

  gameState->level->ticksElapsed++;
}

static void updateMenu(game_state* gameState) {
  //printf("Button selected: %s\n", gameState->menuState->button == PLAY ? "PLAY" : "QUIT");
}

static void updateGameOver(game_state* gameState) {
  // if (GAME_OVER_DELAY - (gameState->level->ticksElapsed - gameState->level->gameOverTick) == 0) {
  //   free_game_state(gameState);
  //   *gameState = *initialiseGameState();
  //   return;
  // }
  //
  // gameState->level->ticksElapsed++;
}

void update(game_state* gameState) {
  switch (gameState->stage) {
    case LEVEL:
      updateLevel(gameState);
      break;
    case MAIN_MENU:
      updateMenu(gameState);
      break;
    case GAME_OVER:
      updateGameOver(gameState);
      break;
  }
}

void printState(game_state* gameState) {

  char* posString = malloc(601 * sizeof(char));

  for (int i = 0; i < 100; i++) {
    posString[i] = '-';
  }

  //show enemies
  list_elem* curr_elem = list_get_first(gameState->enemies);
  while (curr_elem != NULL) {
    enemy_type* enemy = (enemy_type*) curr_elem->value;
    posString[(int) ((enemy->x - 1) / 6)] = 'E';
    curr_elem = list_get_next(curr_elem);
  }

  //show player
  posString[(int) ((gameState->player->x - 1) / 6)] = 'P';

  // printf("%s\n", posString);
  printf("numEnemies: %d, numLeftHit: %d, numRightHit: %d\n", gameState->enemies->size, gameState->leftHitBox->size, gameState->rightHitBox->size);
}

void free_game_state(game_state* gameState) {
  free_player(gameState->player);
  list_destroy(gameState->enemies);
  list_destroy(gameState->leftHitBox);
  list_destroy(gameState->rightHitBox);

  if (gameState->level != NULL) {
    free_level(gameState->level);
  }

  if (gameState->stageRender != NULL) {
    free_stage_render(gameState->stageRender);
  }

  free(gameState->menuState);
  free(gameState);
}
