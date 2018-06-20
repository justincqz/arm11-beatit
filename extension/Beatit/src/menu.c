#include "gamedefs.h"
#include "menu.h"
#include <stdlib.h>

#define NUM_MAIN_BUTTONS 2
#define NUM_GAME_OVER_BUTTONS 2
#define NUM_LEVEL_SELECT_BUTTONS 4

menu_state* initialiseMenuState() {
  menu_state* menuState = malloc(sizeof(menu_state));
  menuState->button = malloc(sizeof(menu_button));
  menuState->button->mainButton = malloc(sizeof(MAIN_MENU_BUTTON));
  *menuState->button->mainButton = MAIN_PLAY;
  return menuState;
}

static GAME_OVER_BUTTON getPrevGameOverButton(GAME_OVER_BUTTON* button) {
  int currIndex = (int) *button;
  currIndex = currIndex == 0 ? NUM_GAME_OVER_BUTTONS : currIndex;
  return (GAME_OVER_BUTTON) ((currIndex - 1) % NUM_GAME_OVER_BUTTONS);
}

static GAME_OVER_BUTTON getNextGameOverButton(GAME_OVER_BUTTON* button) {
  int currIndex = (int) *button;
  return (GAME_OVER_BUTTON) ((currIndex + 1) % NUM_GAME_OVER_BUTTONS);
}

static LEVEL_SELECT_MENU_BUTTON getPrevLevelSelectButton(LEVEL_SELECT_MENU_BUTTON* button) {
  int currIndex = (int) *button;
  currIndex = currIndex == 0 ? NUM_LEVEL_SELECT_BUTTONS : currIndex;
  return (LEVEL_SELECT_MENU_BUTTON) ((currIndex - 1) % NUM_LEVEL_SELECT_BUTTONS);
}

static LEVEL_SELECT_MENU_BUTTON getNextLevelSelectButton(LEVEL_SELECT_MENU_BUTTON* button) {
  int currIndex = (int) *button;
  return (LEVEL_SELECT_MENU_BUTTON) ((currIndex + 1) % NUM_LEVEL_SELECT_BUTTONS);
}

static MAIN_MENU_BUTTON getPrevMainMenuButton(MAIN_MENU_BUTTON* button) {
  int currIndex = (int) *button;
  currIndex = currIndex == 0 ? NUM_MAIN_BUTTONS : currIndex;
  return (MAIN_MENU_BUTTON) ((currIndex - 1) % NUM_MAIN_BUTTONS);
}

static MAIN_MENU_BUTTON getNextMainMenuButton(MAIN_MENU_BUTTON* button) {
  int currIndex = (int) *button;
  return (MAIN_MENU_BUTTON) ((currIndex + 1) % NUM_MAIN_BUTTONS);
}

void selectRightButton(menu_button* button, MENU_TYPE type) {
  switch (type) {
    case MAIN:
      *(button->mainButton) = getNextMainMenuButton(button->mainButton);
      break;
    case GAME_OVER_MENU:
      *(button->gameOverButton) = getNextGameOverButton(button->gameOverButton);
      break;
    case LEVEL_SELECT_MENU:
      *(button->levelSelectButton) = getNextLevelSelectButton(button->levelSelectButton);
      break;
  }
}

void selectLeftButton(menu_button* button, MENU_TYPE type) {
  switch (type) {
    case MAIN:
      *(button->mainButton) = (getPrevMainMenuButton(button->mainButton));
      printf("New button is: %s\n", *button->mainButton == MAIN_PLAY ? "PLAY" : "QUIT");
      break;
    case GAME_OVER_MENU:
      *(button->gameOverButton) = (getPrevGameOverButton(button->gameOverButton));
      break;
    case LEVEL_SELECT_MENU:
      *(button->levelSelectButton) = (getPrevLevelSelectButton(button->levelSelectButton));
      break;
  }
}
