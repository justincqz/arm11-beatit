#include "render.h"

void setSpriteParam(spriteSt_t* st, int frames, spriteSt_t* nextSt, int currentStNo) {
  st->frames = frames;
  st->frames_left = frames;
  st->nextState = nextSt;
  st->currentStNo = currentStNo;
}

void addSpriteSt(render_t* render, spriteSt_t* newSt) {
  render->numStates += 1;
  if (render->numStates == 1) {
    render->states = malloc(sizeof(spriteSt_t*));
  } else {
    render->states = realloc(render->states, render->numStates * sizeof(spriteSt_t*));
  }
  render->states[render->numStates - 1] = newSt;
}

void updateOffset(render_t* r, int x, int y) {
  r->offX = x;
  r->offY = y;
}

// If next state is 0, use default state updating
// If next state is -1, return to default
void processObj(render_t* obj, int8_t nextState, int newX, int newY, Orientation facing) {
  if (obj->currentSt == NULL) {
    return;
  }

  obj->offX = newX;
  obj->offY = newY;
  if (facing != -1) {
    obj->facing = facing;
  }

  if (obj->currentSt->nextState == NULL) {
    return;
  }

  // If same state or trying to go to same state, decrement frames_left
  if (nextState == obj->currentSt->currentStNo || nextState == -1) {
    (obj->currentSt)->frames_left--;

    // If frames_left reached 0, go to next logical state
    if (obj->currentSt->frames_left == 0) {
      spriteSt_t* nxtSt = (obj->currentSt)->nextState;
      nxtSt->frames_left = nxtSt->frames;
      obj->currentSt = nxtSt;
    }

    return;
  }
  // Go to new state
  obj->currentSt = obj->states[nextState];
  obj->currentSt->frames_left = obj->currentSt->frames;
}

int checkIfWhite(pixel_t p) {
  if (p.red == -1 && p.green == -1 && p.blue == -1) {
    return 0;
  }
  return 1;
}

void renderObj(render_t* obj, pixel_t** canvas) {
  if (obj->currentSt == NULL) {
    return;
  }
  pixel_t** sprite = (obj->currentSt)->matrix;
  int dimY = (obj->currentSt)->dimY;
  int offY = obj->offY - (dimY / 2);

  int dimX = (obj->currentSt)->dimX;
  int offX = obj->offX - (dimX / 2);

  for (int y = offY; y < (offY + dimY); y++) {
    if (y < SCREEN_HEIGHT && y > 0) {
      for (int x = offX; x < (offX + dimX); x++) {
        if (x < SCREEN_WIDTH && x > 0) {
          if (obj->facing == LEFT) {
              if (checkIfWhite(sprite[y - offY][(dimX - 1) - (x - offX)]) &&
            !comparePixel(sprite[y - offY][(dimX - 1) - (x - offX)], canvas[y][x])) {
                canvas[y][x] = sprite[y - offY][(dimX - 1) - (x - offX)];
              }
          } else {
              if (checkIfWhite(sprite[y - offY][x - offX]) &&
            !comparePixel(sprite[y - offY][x - offX], canvas[y][x])) {
                canvas[y][x] = sprite[y - offY][x - offX];
              }
          }
        }
      }
    }
  }
}

void updateCanvas(pixel_t** oldCanvas, pixel_t** currCanvas) {
  int updatePix = 0;
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      if (!comparePixel(currCanvas[y][x], oldCanvas[y][x])) {
        gfx_color(currCanvas[y][x].red, currCanvas[y][x].green, currCanvas[y][x].blue);
        gfx_point(x, y);
        updatePix++;
      }
    }
  }
}

render_t* init_render(char* path, int offx, int offy) {
  spriteSt_t* sprite = readbmp(path);
  render_t* render = new_render();
  addSpriteSt(render, sprite);
  render->currentSt = sprite;
  updateOffset(render, offx, offy);
  return render;
}

spriteSt_t* copy_sprite(const spriteSt_t* sp) {
  spriteSt_t* ret = new_spriteSt();
  ret->matrix = cpyPixelMatrix(sp->matrix, sp->dimY, sp->dimX);
  ret->dimX = sp->dimX;
  ret->dimY = sp->dimY;
  ret->frames = sp->frames;
  ret->frames_left = ret->frames;
  ret->currentStNo = sp->currentStNo;
  return ret;
}

int comparePixel(pixel_t px1, pixel_t px2) {
  return (px1.red == px2.red) && (px1.green == px2.green) && (px1.blue == px2.blue);
}
