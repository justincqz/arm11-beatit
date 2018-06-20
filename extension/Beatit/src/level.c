#include "list.h"
#include "gamedefs.h"
#include <stdlib.h>
#include "renderinit.h"

void free_level(level_t* level) {
  list_destroy(level->spawns);
  free(level);
}

spawn_details* alloc_spawn_detail() {
  spawn_details* new_spawn = malloc(sizeof(spawn_details));

  if (new_spawn == NULL) {
    perror("Unable to allocate memory for spawn details!\n");
    exit(EXIT_FAILURE);
  }

  return new_spawn;
}

spawn_details* create_spawn_detail(SPAWN_LOC spawnDir, double initialX, double initialY, long numTicksBeforeSpawn) {
  spawn_details* new_spawn = alloc_spawn_detail();

  new_spawn->spawnDir = spawnDir;
  new_spawn->initialX = initialX;
  new_spawn->initialY = initialY;
  new_spawn->numTicksBeforeSpawn = numTicksBeforeSpawn;

  return new_spawn;
}

level_t* create_level_one(void) {

  level_t* level = malloc(sizeof(level_t));

  spawn_details* firstSpawn = create_spawn_detail(LEFT_OF_MC, 0, SCREEN_HEIGHT / 2, 10);
  spawn_details* secondSpawn = create_spawn_detail(RIGHT_OF_MC, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 20);
  spawn_details* thirdSpawn = create_spawn_detail(RIGHT_OF_MC, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 30);
  spawn_details* fourthSpawn = create_spawn_detail(LEFT_OF_MC, 0, SCREEN_HEIGHT / 2, 40);
  spawn_details* fifthSpawn = create_spawn_detail(RIGHT_OF_MC, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 50);

  level->spawns = create_list();
  list_insert(level->spawns, firstSpawn);
  list_insert(level->spawns, secondSpawn);
  list_insert(level->spawns, thirdSpawn);
  list_insert(level->spawns, fourthSpawn);
  list_insert(level->spawns, fifthSpawn);

  level->levelNum = LEVEL_ONE;
  level->ticksElapsed = 0;
  level->totalEnemies = 5;
  level->numEnemies = 5;
  level->score = 0;
  level->combo = 0;
  return level;
}

void initialiseLevel(game_state* gameState, LEVEL_NUM levelNum) {
  switch (levelNum) {
    case LEVEL_ONE:
      gameState->level = create_level_one();
      gameState->stageRender = init_level_one();
      break;
    case LEVEL_TWO:
    case LEVEL_THREE:
    case LEVEL_FOUR:
    default:
      printf("Level unable to initialise.\n");
  }
}

void reinitialiseCurrLevel(game_state* gameState) {

  LEVEL_NUM levelNum = gameState->level->levelNum;
  free_level(gameState->level);

  switch (levelNum) {
    case LEVEL_ONE:
      gameState->level = create_level_one();
      break;
    case LEVEL_TWO:
    case LEVEL_THREE:
    case LEVEL_FOUR:
    default:
      printf("Unable to restart this level!\n");
  }
}

void updateScore(level_t* level, HIT_TYPE hitType) {
  switch (hitType) {
    case HIT:
      level->score += (900000 / level->totalEnemies + 200000 / ((level->totalEnemies) * (level->totalEnemies - 1)) * level->combo);
      break;
    case MISS:
      break;
    default:
      printf("Invalid HIT_TYPE!\n");
      break;
  }

  printf("numEnemies: %d, combo: %d, Current level score: %u\n", level->numEnemies, level->combo, level->score);
}
