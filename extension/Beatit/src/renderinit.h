#ifndef RENDERINIT_H
#define RENDERINIT_H

#include "renderdef.h"
#include "renderio.h"
#include "render.h"
#include <stdlib.h>

render_t* initializeMCRender(int x, int y);

render_t* initializeEnemyRender(int x, int y, Orientation facing);

render_t* init_digits();

void render_score(int score, int offX, int offY, pixel_t** canvas, render_t* digit);

render_t* init_beat_render(int x, int y, Orientation facing);

stage_render_t* init_main_menu();

stage_render_t* init_level_one();

stage_render_t* init_game_over();

stage_render_t* init_level_select();

#endif
