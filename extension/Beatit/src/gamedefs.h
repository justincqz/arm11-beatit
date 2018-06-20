#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define HITBOX_HEIGHT 425
#define ENEMY_FLY_SPEED_DX 30
#define ENEMY_FLY_SPEED_DY 100
#define ENEMY_MOVE_SPEED 10
#define FLOOR_Y 300
#define PLAYER_ATTACK_COOLDOWN 15


#include "list.h"
#include "render.h"
#include "renderdef.h"

typedef enum {
  PLAY,
  QUIT
} MENU_BUTTON;

typedef enum {
  MAIN_PLAY,
  MAIN_QUIT
} MAIN_MENU_BUTTON;

typedef enum {
  GAME_OVER_RETRY,
  GAME_OVER_QUIT
} GAME_OVER_BUTTON;

typedef enum {
  LEVEL_SELECT_ONE,
  LEVEL_SELECT_TWO,
  LEVEL_SELECT_THREE,
  LEVEL_SELECT_FOUR
} LEVEL_SELECT_MENU_BUTTON;

typedef enum {
  LEVEL_ONE,
  LEVEL_TWO,
  LEVEL_THREE,
  LEVEL_FOUR
} LEVEL_NUM;

typedef enum {
  MAIN,
  GAME_OVER_MENU,
  LEVEL_SELECT_MENU
} MENU_TYPE;

typedef union menu_button {
  MAIN_MENU_BUTTON* mainButton;
  GAME_OVER_BUTTON* gameOverButton;
  LEVEL_SELECT_MENU_BUTTON* levelSelectButton;
} menu_button;

typedef struct menu_state {
  menu_button* button;
} menu_state;

typedef enum {
  MOVE,
  ATTACK,
  RECEIVING_ATTACK,
  GETTING_READY,
  DIE
} ACTION;

typedef struct speed_type {
  double dx;
  double dy;
} speed_type;

typedef enum STATUS {
  DEAD = 1,
  ALIVE = 0
} STATUS;

typedef struct player_cooldowns {
  int deathCounter;
  int attackCounter;
} player_cooldowns;

typedef struct player_type {
  double x;
  double y;
  double range;
  render_t* render;
  int lives;
  player_cooldowns* cooldowns;
} player_type;

typedef enum SPAWN_LOC {
  LEFT_OF_MC,
  RIGHT_OF_MC
} SPAWN_LOC;

typedef struct enemy_cooldowns {
  int deathCounter;
  int attackCounter;
} enemy_cooldowns;

typedef struct enemy_type {
  double x;
  double y;
  double range;
  speed_type* speed;
  render_t* render;
  render_t* beat;
  STATUS status;
  ACTION action;
  enemy_cooldowns* cooldowns;
} enemy_type;

typedef struct spawn_details {
  SPAWN_LOC spawnDir;
  double initialX;
  double initialY;
  long numTicksBeforeSpawn;
} spawn_details;

typedef struct level_t {
  list* spawns;
  long ticksElapsed;
  int numEnemies;
  int totalEnemies;
  uint32_t score;
  uint32_t combo;
  long gameOverTick;
  LEVEL_NUM levelNum;
} level_t;

typedef enum {
  LEVEL = 1,
  MAIN_MENU = 0,
  GAME_OVER = 2,
  LEVEL_SELECT
} STAGE_OF_GAME;

typedef struct Game_State {
  player_type* player;
  list* enemies;
  list* leftHitBox;
  list* rightHitBox;
  level_t* level;
  STAGE_OF_GAME stage;
  menu_state* menuState;
  stage_render_t* stageRender;
} game_state;

typedef enum HIT_TYPE {
  HIT,
  MISS
} HIT_TYPE;

#endif
