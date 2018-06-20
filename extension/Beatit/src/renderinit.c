#include "renderinit.h"

render_t* initializeMCRender(int x, int y) {

  /* MJ Idle */
  spriteSt_t* idle1 = readbmp("../assets/char/mc/idle1.bmp");
  spriteSt_t* idle2 = readbmp("../assets/char/mc/idle2.bmp");
  printf("Loaded Idles!\n");
  setSpriteParam(idle1, 8, idle2, 0);
  setSpriteParam(idle2, 8, idle1, 0);

  /* MJ Kick */
  spriteSt_t* kick1 = readbmp("../assets/char/mc/kick1.bmp");
  spriteSt_t* kick2 = readbmp("../assets/char/mc/kick2.bmp");
  spriteSt_t* kick3 = readbmp("../assets/char/mc/kick3.bmp");
  spriteSt_t* kick4 = readbmp("../assets/char/mc/kick4.bmp");
  //spriteSt_t* kick5 = readbmp("../assets/char/mc/kick5.bmp");
  printf("Loaded kicks!\n");
  spriteSt_t* kick6 = copy_sprite(kick3);
  spriteSt_t* kick7 = copy_sprite(kick2);

  setSpriteParam(kick1, 1, kick2, 2);
  setSpriteParam(kick2, 2, kick4, 2);
  setSpriteParam(kick3, 1, kick4, 2);
  setSpriteParam(kick4, 2, kick6, 2);
  //setSpriteParam(kick5, 2, kick6);
  setSpriteParam(kick6, 1, kick7, 2);


  /* MJ Punch */
  spriteSt_t* punch1 = readbmp("../assets/char/mc/punch1.bmp");
  spriteSt_t* punch2 = readbmp("../assets/char/mc/punch2.bmp");
  spriteSt_t* punch3 = readbmp("../assets/char/mc/punch3.bmp");
  printf("Loaded punches!\n");

  setSpriteParam(kick7, 1, idle1, 2);

  setSpriteParam(punch1, 1, punch3, 1);
  setSpriteParam(punch2, 1, idle1, 1);
  setSpriteParam(punch3, 2, punch2, 1);

  /* MJ Render */
  render_t* ret = new_render();
  addSpriteSt(ret, idle1);
  addSpriteSt(ret, idle2);
  addSpriteSt(ret, kick1);
  addSpriteSt(ret, kick2);
  addSpriteSt(ret, kick3);
  addSpriteSt(ret, kick4);
  //addSpriteSt(ret, kick5);
  addSpriteSt(ret, kick6);
  addSpriteSt(ret, kick7);
  addSpriteSt(ret, punch1);
  addSpriteSt(ret, punch2);
  addSpriteSt(ret, punch3);
  ret->currentSt = punch1;

  updateOffset(ret, x, y);
  return ret;
}

render_t* initializeEnemyRender(int x, int y, Orientation facing) {
  /* Enemy walk */
  spriteSt_t* walk1 = readbmp("../assets/char/enemy1/walk1.bmp");
  spriteSt_t* walk2 = readbmp("../assets/char/enemy1/walk2.bmp");
  spriteSt_t* walk3 = readbmp("../assets/char/enemy1/walk3.bmp");
  spriteSt_t* walk4 = readbmp("../assets/char/enemy1/walk4.bmp");

  spriteSt_t* dead1 = readbmp("../assets/char/enemy1/dead1.bmp");
  spriteSt_t* dead2 = readbmp("../assets/char/enemy1/dead2.bmp");
  spriteSt_t* dead3 = readbmp("../assets/char/enemy1/dead3.bmp");
  spriteSt_t* dead4 = readbmp("../assets/char/enemy1/dead4.bmp");

  setSpriteParam(walk1, 1, walk2, 0);
  setSpriteParam(walk2, 2, walk3, 0);
  setSpriteParam(walk3, 2, walk1, 0);
  setSpriteParam(walk4, 2, walk1, 0);

  setSpriteParam(dead1, 1, dead2, 1);
  setSpriteParam(dead2, 1, dead3, 1);
  setSpriteParam(dead3, 2, dead4, 1);
  setSpriteParam(dead4, 4, dead4, 1);

  render_t* ret = new_render();
  addSpriteSt(ret, walk1);
  addSpriteSt(ret, dead1);
  addSpriteSt(ret, walk2);
  addSpriteSt(ret, walk3);
  addSpriteSt(ret, walk4);
  addSpriteSt(ret, dead2);
  addSpriteSt(ret, dead3);
  addSpriteSt(ret, dead4);

  ret->currentSt = walk1;
  ret->facing = facing;
  return ret;
}

// TODO: Score thingy
render_t* init_digits() {
  render_t* ret = new_render();

  spriteSt_t* zer = readbmp("../assets/digits/0.bmp");
  spriteSt_t* one = readbmp("../assets/digits/1.bmp");
  spriteSt_t* two = readbmp("../assets/digits/2.bmp");
  spriteSt_t* thr = readbmp("../assets/digits/3.bmp");
  spriteSt_t* fou = readbmp("../assets/digits/4.bmp");
  spriteSt_t* fiv = readbmp("../assets/digits/5.bmp");
  spriteSt_t* six = readbmp("../assets/digits/6.bmp");
  spriteSt_t* sev = readbmp("../assets/digits/7.bmp");
  spriteSt_t* eig = readbmp("../assets/digits/8.bmp");
  spriteSt_t* nin = readbmp("../assets/digits/9.bmp");

  setSpriteParam(zer, 10, zer, 0);
  setSpriteParam(one, 10, one, 1);
  setSpriteParam(two, 10, two, 2);
  setSpriteParam(thr, 10, thr, 3);
  setSpriteParam(fou, 10, fou, 4);
  setSpriteParam(fiv, 10, fiv, 5);
  setSpriteParam(six, 10, six, 6);
  setSpriteParam(sev, 10, sev, 7);
  setSpriteParam(eig, 10, eig, 8);
  setSpriteParam(nin, 10, nin, 9);
  addSpriteSt(ret, zer);
  addSpriteSt(ret, one);
  addSpriteSt(ret, two);
  addSpriteSt(ret, thr);
  addSpriteSt(ret, fou);
  addSpriteSt(ret, fiv);
  addSpriteSt(ret, six);
  addSpriteSt(ret, sev);
  addSpriteSt(ret, eig);
  addSpriteSt(ret, nin);
  ret->currentSt = zer;
  ret->facing = RIGHT;

  return ret;
}

render_t* init_beat_render(int x, int y, Orientation facing) {
  render_t* ret = new_render();

  spriteSt_t* beatLeftA = readbmp("../assets/panel/beatbutton1.bmp");
  spriteSt_t* beatLeftB = readbmp("../assets/panel/beatbutton2.bmp");
  spriteSt_t* beatRightA = readbmp("../assets/panel/beatbutton3.bmp");
  spriteSt_t* beatRightB = readbmp("../assets/panel/beatbutton4.bmp");
  setSpriteParam(beatLeftA, 1, beatLeftA, 0);
  setSpriteParam(beatLeftB, 1, NULL, 1);
  setSpriteParam(beatRightA, 1, beatRightA, 0);
  setSpriteParam(beatRightB, 1, NULL, 1);
  addSpriteSt(ret, beatLeftA);
  addSpriteSt(ret, beatLeftB);
  addSpriteSt(ret, beatRightA);
  addSpriteSt(ret, beatRightB);
  updateOffset(ret, x, y);
  ret->facing = facing;
  ret->currentSt = facing == RIGHT ? beatLeftA : beatRightA;

  return ret;
}

stage_render_t* init_main_menu() {
  render_t* bg = init_render("../assets/bg/mainmenubg2.bmp", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  render_t* playButton = init_render("../assets/bg/menu_button1.bmp", 178, 300);
  spriteSt_t* play1 = copy_sprite(playButton->currentSt);
  spriteSt_t* play2 = readbmp("../assets/bg/menu_button2.bmp");
  setSpriteParam(playButton->currentSt, 10, playButton->currentSt, 0);
  setSpriteParam(play1, 1, play2, 1);
  setSpriteParam(play2, 10, play2, 1);
  addSpriteSt(playButton, play1);
  addSpriteSt(playButton, play2);

  render_t* quitButton = init_render("../assets/bg/menu_button3.bmp", 622, 300);
  spriteSt_t* quit1 = copy_sprite(quitButton->currentSt);
  spriteSt_t* quit2 = readbmp("../assets/bg/menu_button4.bmp");
  setSpriteParam(quitButton->currentSt, 10, quitButton->currentSt, 0);
  setSpriteParam(quit1, 1, quit2, 1);
  setSpriteParam(quit2, 10, quit2, 1);
  addSpriteSt(quitButton, quit1);
  addSpriteSt(quitButton, quit2);

  stage_render_t* ret = new_stage_render();
  ret->bg = bg;
  ret->button1 = playButton;
  ret->button2 = quitButton;
  return ret;
}

stage_render_t* init_level_one() {
  render_t* bg = init_render("../assets/bg/bg1.bmp", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  render_t* panel = init_render("../assets/panel/panel.bmp", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  stage_render_t* ret = new_stage_render();
  ret->bg = bg;
  ret->panel = panel;
  return ret;
}

stage_render_t* init_game_over() {
  render_t* retry = init_render("../assets/bg/gameover1.bmp", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  spriteSt_t* title = readbmp("../assets/bg/gameover2.bmp");
  setSpriteParam(retry->currentSt, 10, retry->currentSt, 0);
  setSpriteParam(title, 10, title, 1);
  addSpriteSt(retry, title);

  stage_render_t* ret = new_stage_render();
  ret->bg = retry;
  return ret;
}

stage_render_t* init_level_select() {
  render_t* bg = init_render("../assets/bg/stageselectbgb.bmp", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  render_t* panel = init_render("../assets/bg/stageselectbgf.bmp", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  render_t* button1 = init_render("../assets/panel/arrowButton.bmp", 20, 280);
  render_t* button2 = init_render("../assets/panel/arrowButton.bmp", 780, 280);
  button1->facing = LEFT;

  // 400, 262
  render_t* pic = new_render();
  spriteSt_t* piclevelone = readbmp("../assets/bg/levelselect1.bmp");
  spriteSt_t* picleveltwo = readbmp("../assets/bg/levelselect2.bmp");
  spriteSt_t* piclevelthree = readbmp("../assets/bg/levelselect3.bmp");
  spriteSt_t* piclevelfour = readbmp("../assets/bg/levelselect4.bmp");
  setSpriteParam(piclevelone, 10, piclevelone, 0);
  setSpriteParam(picleveltwo, 10, picleveltwo, 1);
  setSpriteParam(piclevelthree, 10, piclevelthree, 2);
  setSpriteParam(piclevelfour, 10, piclevelfour, 3);
  addSpriteSt(pic, piclevelone);
  addSpriteSt(pic, picleveltwo);
  addSpriteSt(pic, piclevelthree);
  addSpriteSt(pic, piclevelfour);
  updateOffset(pic, 400, 262);
  pic->currentSt = piclevelone;

  stage_render_t* ret = new_stage_render();
  ret->bg = bg;
  ret->button1 = button1;
  ret->button2 = button2;
  ret->panel = panel;
  ret->pic = pic;

  return ret;
}

void render_score(int score, int offX, int offY, pixel_t** canvas, render_t* digit) {
  int oX = offX + 84;
  int oY = offY - 20;
  int runningScore = score;

  // Right to left
  for (int d = 0; d < 7; d++) {
    int currDigit = runningScore % 10;
    processObj(digit, currDigit, (oX - (d * 24)), oY, RIGHT);
    renderObj(digit, canvas);
    runningScore = runningScore / 10;
  }
}

void render_text(char* text, int offX, int offY, pixel_t** canvas, render_t* chars) {

}
