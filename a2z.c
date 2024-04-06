#include "keycheck.h"
#include <time.h>

int main() {
  Keyboard kbd;
  Keyboard_(&kbd);

  time_t t = time(NULL);

  int c = 'a';
  while (c <= 'z') {
    int k = Keyboard_inkey(&kbd);
    if (k == 0) {
      continue;
    }
    if (k != c) {
      putchar('\x08');
      putchar('\a');
      fflush(stdout);
      continue;
    }
    c = c + 1;
  }

  printf("\n%d\n", (int)(time(NULL) - t));

  Keyboard__(&kbd);
  return 0;
}
