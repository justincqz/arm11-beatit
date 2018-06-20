#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>
#include <stdint.h>
#include "gamedefs.h"
#include "renderdef.h"
#include "gfx.h"
#include "renderio.h"

#define NUM_MILLISECONDS 60
 
void setSpriteParam(spriteSt_t* st, int frames, spriteSt_t* nextSt, int currStNo);

void addSpriteSt(render_t* render, spriteSt_t* newSt);

void updateOffset(render_t* r, int x, int y);

// If next state is -1, use default state updating
void processObj(render_t* obj, int8_t nextState, int newX, int newY, Orientation facing);

int checkIfWhite(pixel_t p);

void renderObj(render_t* obj, pixel_t** canvas);

void updateCanvas(pixel_t** oldCanvas, pixel_t** currCanvas);

int comparePixel(pixel_t px1, pixel_t px2);

render_t* init_render(char* path, int offx, int offy);

spriteSt_t* copy_sprite(const spriteSt_t* r);

#endif
