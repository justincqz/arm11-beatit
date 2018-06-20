#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "gfx.h"
#include "render.h"
#include "renderinit.h"
#include "renderdef.h"
#include "renderio.h"
#include "list.h"
#include "enemy.h"
#include "player.h"
#include "gamelogic.h"
#include "menu.h"
#include "level.h"

void exit_game(game_state* gameState, pixel_t **canvas) {
  free_game_state(gameState);
  freePixelMatrix(canvas);
  exit(EXIT_SUCCESS);
}

void handle_input_level(char input, game_state* gameState, int8_t* inputBuffer, Orientation* orientBuffer) {
  switch (input) {
    case 'a':
      if (attackLeft(gameState)) {
        *inputBuffer = 8;
        *orientBuffer = LEFT;
      }
      break;
    case 's':
      if (attackRight(gameState)) {
        *inputBuffer = 8;
        *orientBuffer = RIGHT;
      }
      break;
    default:
      printf("Unregistered input...\n");
  }
}

void handle_input_game_over(char input, game_state* gameState, pixel_t **canvas) {
  switch (input) {
    case 'a':
      selectLeftButton(gameState->menuState->button, GAME_OVER_MENU);
      printf("selected button: %s\n", *(gameState->menuState->button->gameOverButton) == GAME_OVER_RETRY ? "RETRY" : "QUIT");
      break;
    case 's':
      selectRightButton(gameState->menuState->button, GAME_OVER_MENU);
      printf("selected button: %s\n", *(gameState->menuState->button->gameOverButton) == GAME_OVER_RETRY ? "RETRY" : "QUIT");
      break;
    case 'q':
      if (*gameState->menuState->button->gameOverButton == GAME_OVER_RETRY) {
        reinitialiseCurrLevel(gameState);
        gameState->stage = LEVEL;
      } else if (*gameState->menuState->button->gameOverButton == GAME_OVER_QUIT) {
        exit_game(gameState, canvas);
      }
      break;
    default:
      printf("Unregistered input...\n");
  }
}

void handle_input_main_menu(char input, game_state* gameState, pixel_t **canvas) {
  switch (input) {
    case 'a':
      selectLeftButton(gameState->menuState->button, MAIN);
      break;
    case 's':
      selectRightButton(gameState->menuState->button, MAIN);
      break;
    case 'q':
      if (*gameState->menuState->button->mainButton == MAIN_PLAY) {
        //initialise render of level select, set stage to level_select.

        gameState->stage = LEVEL_SELECT;
      } else if (*gameState->menuState->button->mainButton == MAIN_QUIT) {
        exit_game(gameState, canvas);
      }
      break;
    default:
      printf("Unregistered input...\n");
  }
}

void handle_input_level_select(char input, game_state* gameState) {
  switch (input) {
    case 'a':
      selectLeftButton(gameState->menuState->button, LEVEL_SELECT_MENU);
      printf("selected button: %s\n", *gameState->menuState->button->levelSelectButton == LEVEL_SELECT_ONE ? "LEVEL ONE" : "NOT LEVEL ONE");
      break;
    case 's':
      selectRightButton(gameState->menuState->button, LEVEL_SELECT_MENU);
      printf("selected button: %s\n", *gameState->menuState->button->levelSelectButton == LEVEL_SELECT_ONE ? "LEVEL ONE" : "NOT LEVEL ONE");
      break;
    case 'q':

      switch (*gameState->menuState->button->levelSelectButton) {
        case LEVEL_SELECT_ONE:
          initialiseLevel(gameState, LEVEL_ONE);
          break;
        case LEVEL_SELECT_TWO:
          // initialiseLevel(gameState, LEVEL_TWO);
          // break;
        case LEVEL_SELECT_THREE:
          // initialiseLevel(gameState, LEVEL_THREE);
          // break;
        case LEVEL_SELECT_FOUR:
          // initialiseLevel(gameState, LEVEL_FOUR);
          // break;
        default:
          printf("Level one NOT selected! try again!\n");
      }
      gameState->stage = LEVEL;

      break;
    case 'e':
      *gameState->menuState->button->mainButton = MAIN_PLAY;
      gameState->stage = MAIN_MENU;
      break;
    default:
      printf("Unregistered input...\n");
  }
}

int main(int argc, char** argv) {

    gfx_open(SCREEN_WIDTH, SCREEN_HEIGHT, "Main Game Frame");


    game_state* gameState = initialiseGameState();
    pixel_t **canvas = allocPixelMatrix(SCREEN_HEIGHT, SCREEN_WIDTH);
    render_t* digits = init_digits();
    stage_render_t* mainMenu = init_main_menu();
    stage_render_t* gameOver = init_game_over();
    stage_render_t* levelSelect = init_level_select();

    // Test Purposes
    /*
    spriteSt_t* testSprite = readbmp("../assets/char/mc/idle1.bmp");
    render_t* testSp = new_render();
    addSpriteSt(testSp, testSprite);
    testSp->currentSt = testSprite;
    testSp->offX = 800;
    testSp->offY = 300;
    */

    double time_counter = 0;
    clock_t this_time = clock();
    clock_t last_time = this_time;
    stage_render_t* currSt;

    int8_t inputBuffer = 0;
    Orientation orientBuffer = LEFT;


    while(1) {
        this_time = clock();
        time_counter += (double)(this_time - last_time);
        last_time = this_time;

        if (gfx_event_waiting()) {
          char input = gfx_wait();
          printf("Detected an input of %c!\n", input);

            switch (gameState->stage) {
              case LEVEL:
                handle_input_level(input, gameState, &inputBuffer, &orientBuffer);
                break;
              case MAIN_MENU:
                handle_input_main_menu(input, gameState, canvas);
                break;
              case GAME_OVER:
                handle_input_game_over(input, gameState, canvas);
                break;
              case LEVEL_SELECT:
                handle_input_level_select(input, gameState);
                break;
            }
        }

        if(time_counter > (double)(NUM_MILLISECONDS * 0.001 * CLOCKS_PER_SEC)) {
            time_counter -= (double)(NUM_MILLISECONDS * 0.001 * CLOCKS_PER_SEC);

            // Game logic
            update(gameState);
            // printState(gameState);

            //rendering logic.
            pixel_t** prevCanvas = cpyPixelMatrix(canvas, SCREEN_HEIGHT, SCREEN_WIDTH);
            switch (gameState->stage) {
              case LEVEL:
                renderObj(gameState->stageRender->bg, canvas);
                renderObj(gameState->stageRender->panel, canvas);

                list_elem* curr_elem = list_get_first(gameState->enemies);

                enemy_type* enemy;
                int x = 0;
                int y = 0;
                while (curr_elem != NULL) {
                  enemy = (enemy_type*) curr_elem->value;
                  processObj(enemy->render, enemy->status, enemy->x, enemy->y, -1);
                  if (enemy->status == ALIVE) {
                    processObj(enemy->beat, 0, enemy->x, HITBOX_HEIGHT, enemy->beat->facing);
                  }
                  renderObj(enemy->render, canvas);
                  renderObj(enemy->beat, canvas);
                  curr_elem = list_get_next(curr_elem);
                }
                processObj(gameState->player->render, inputBuffer, gameState->player->x, gameState->player->y, orientBuffer);
                renderObj(gameState->player->render, canvas);

                // Load Score
                render_score(gameState->level->score, SCREEN_WIDTH / 2, 525, canvas, digits);

                updateCanvas(prevCanvas, canvas);
                inputBuffer = -1;
                break;

              case MAIN_MENU:
                currSt = mainMenu;
                renderObj(currSt->bg, canvas);
                processObj(currSt->button2, *(gameState->menuState->button->mainButton), 562, 521, RIGHT);
                processObj(currSt->button1, !*(gameState->menuState->button->mainButton), 240, 521, RIGHT);

                renderObj(currSt->button1, canvas);
                renderObj(currSt->button2, canvas);
                updateCanvas(prevCanvas, canvas);
                break;

              case LEVEL_SELECT:
                currSt = levelSelect;
                renderObj(currSt->bg, canvas);
                renderObj(currSt->button1, canvas);
                renderObj(currSt->button2, canvas);
                processObj(currSt->pic, *(gameState->menuState->button->levelSelectButton), 400, 262, RIGHT);
                renderObj(currSt->pic, canvas);
                //renderObj(currSt->panel, canvas);
                updateCanvas(prevCanvas, canvas);
                break;

              case GAME_OVER:
                currSt = gameOver;
                processObj(currSt->bg, *(gameState->menuState->button->gameOverButton), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, RIGHT);
                renderObj(currSt->bg, canvas);

                render_score(gameState->level->score, 335, 265, canvas, digits);
                updateCanvas(prevCanvas, canvas);
                break;
            }
            freePixelMatrix(prevCanvas);
        }
    }
}
