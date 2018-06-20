#ifndef MENU_H
#define MENU_H

#include "gamedefs.h"
#include "menu.h"
#include <stdlib.h>

#define NUM_MAIN_BUTTONS 2
#define NUM_GAME_OVER_BUTTONS 2
#define NUM_LEVEL_SELECT_BUTTONS 4

menu_state* initialiseMenuState();

void selectRightButton(menu_button* button, MENU_TYPE type);

void selectLeftButton(menu_button* button, MENU_TYPE type);

#endif
