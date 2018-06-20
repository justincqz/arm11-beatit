#include "renderdef.h"

pixel_t** allocPixelMatrix(unsigned int rows, unsigned int cols) {
  pixel_t** matrix;
  unsigned int i;
  matrix = (pixel_t**) malloc (rows * sizeof(pixel_t*));
  if (!matrix) {
    return NULL;
  }
  matrix[0] = (pixel_t*) malloc (rows*cols*sizeof(pixel_t));
  if (!matrix[0]) {
    free(matrix);
    return NULL;
  }
  for (i = 1; i < rows; i++) {
    matrix[i] = matrix[i-1] + cols;
  }
  return matrix;
}

pixel_t** cpyPixelMatrix(pixel_t** m, unsigned int rows, unsigned int cols) {
  pixel_t** ret = allocPixelMatrix(rows, cols);
  ret[0] = memcpy(ret[0], m[0], sizeof(pixel_t) * rows * cols);
  return ret;
}

void freePixelMatrix(pixel_t** matrix) {
  free(matrix[0]);
  free(matrix);
}

spriteSt_t* new_spriteSt() {
  return malloc(sizeof(spriteSt_t));
}

void free_spriteSt(spriteSt_t* st) {
  freePixelMatrix(st->matrix);
  free(st);
}

render_t* new_render() {
  render_t* ret = malloc(sizeof(render_t));
  ret->numStates = 0;
  ret->offX = 0;
  ret->offY = 0;
  ret->currentSt = NULL;
  ret->facing = RIGHT;
  return ret;
}

void free_render(render_t* r) {
  for (int i = 0; i < r->numStates; i++) {
    free_spriteSt((r->states)[i]);
  }
  free(r->states);
  free(r);
}

stage_render_t* new_stage_render() {
  stage_render_t* ret = malloc(sizeof(stage_render_t));
  if (!ret) {
    printf("Error malloc\n");
    exit(EXIT_FAILURE);
  }
  ret->bg = NULL;
  ret->panel = NULL;
  ret->button1 = NULL;
  ret->button2 = NULL;
  ret->pic = NULL;
  return ret;
}

void free_stage_render(stage_render_t* sr) {
  if (sr->bg) {
    free_render(sr->bg);
  }

  if (sr->panel) {
    free_render(sr->panel);
  }

  if (sr->button1) {
    free_render(sr->button1);
  }

  if (sr->button2) {
    free_render(sr->button2);
  }

  if (sr->pic) {
    free_render(sr->pic);
  }
  free(sr);
}
