#include "kawakudari.h"

Image resNeko = {
  .w   = 7,
  .h   = 3,
  .buf = " ^___^ "
         "| '_' |"
         " ----- "
};

Image resTeki = {
  .w   = 1,
  .h   = 1,
  .buf = "*"
};

#define TEKI_SPEED 200
#define N_TEKI 200

#define KEY_UP  65
#define KEY_DOWN  66
#define KEY_RIGHT 67
#define KEY_LEFT  68

int main() {
  Screen scr;
  Keyboard kbd;

  Sprite neko;
  Sprite tekis[N_TEKI];

  Screen_(&scr);
  Keyboard_(&kbd);

  int x = 15;
  int y = 5;
  Sprite_(&neko, &resNeko, Sprite_tick_dummy);
  neko.x_milli = x * 1000;
  neko.y_milli = y * 1000;
  Screen_addSprite(&scr, &neko);

  for (int i = 0; i < N_TEKI; i++) {
    Sprite_(&tekis[i], &resTeki, Sprite_tick_dummy);
    tekis[i].x_milli = (rand() % SCR_W) * 1000;
    tekis[i].y_milli = (SCR_H + i) * 1000;
    Screen_addSprite(&scr, &tekis[i]);
  }

  int gameflg = 1;
  while (gameflg) {
    char c = Keyboard_inkey(&kbd);
    if (c == '\e') {
      gameflg = 0;
    } else if (c == KEY_RIGHT) {
      x = x + 1;
    } else if (c == KEY_LEFT) {
      x = x - 1;
    } else if (c == KEY_UP) {
      y = y - 1;
    } else if (c == KEY_DOWN) {
      y = y + 1;
    }
    neko.x_milli = x * 1000;
    neko.y_milli = y * 1000;

    for (int i = 0; i < N_TEKI; i++) {
      tekis[i].y_milli -= TEKI_SPEED;
      if (tekis[i].y_milli < 0) {
        tekis[i].y_milli += N_TEKI * 1000;
      }
      int tx = tekis[i].x_milli / 1000;
      int ty = tekis[i].y_milli / 1000;
      if (tx >= x && tx < x + resNeko.w && ty >= y && ty < y + resNeko.h) {
        gameflg = 0;
        break;
      }
    }
    Screen_draw(&scr);
    usleep(10000);    
  }
  usleep(1000000);
  Keyboard__(&kbd);
  Screen__(&scr);
  return 0;
}
