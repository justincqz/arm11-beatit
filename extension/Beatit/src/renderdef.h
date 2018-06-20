#ifndef RENDER_DEF
#define RENDER_DEF
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define CUSTOM_TIT_X 64
#define CUSTOM_TIT_Y 458
#define MAX_CHAR 20


typedef enum {
  LEFT = 0,
  RIGHT = 1
} Orientation;

typedef struct Pixel{
    char red, green, blue;
} pixel_t;

typedef struct spriteSt {
  pixel_t **matrix;
  uint16_t dimX;
  uint16_t dimY;
  struct spriteSt* nextState;
  uint16_t frames;
  uint16_t frames_left;
  uint16_t currentStNo;
} spriteSt_t;

typedef struct render {
  int16_t offX;
  int16_t offY;
  spriteSt_t** states;
  spriteSt_t* currentSt;
  size_t numStates;
  Orientation facing;
} render_t;

typedef struct stage_render {
  render_t* bg;
  render_t* panel;
  render_t* button1;
  render_t* button2;
  render_t* pic;
} stage_render_t;

pixel_t** allocPixelMatrix(unsigned int rows, unsigned int cols);

void freePixelMatrix(pixel_t** matrix);

pixel_t** cpyPixelMatrix(pixel_t** m, unsigned int rows, unsigned int cols);

spriteSt_t* new_spriteSt();

void free_spriteSt(spriteSt_t* st);

render_t* new_render();

void free_render(render_t* r);

stage_render_t* new_stage_render();

void free_stage_render(stage_render_t* sr);

#endif
