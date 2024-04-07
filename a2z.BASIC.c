#include "keycheck.h"
#include <time.h>

int main() {
  Keyboard kbd;
  Keyboard_(&kbd);
  setbuf(stdout, NULL);

  //-- like a BASIC
  time_t t = time(NULL);                        // CLT
  int c = 'a';                                  // C=ASC("A")
  Line30:{
  int k = Keyboard_inkey(&kbd);                 // K=INKEY()
  if (k == 0){ goto Line30; }                   // IF K=0 GOTO 30
  if (k != c){ putchar('\a'); goto Line30; }    // IF K!=C BEEP:GOTO 30
  putchar(c);                                   // ?CHR$(C);
  c = c + 1;                                    // C=C+1
  if (c <= 'z') goto Line30;                    // IF C<=ASC("Z") GOTO 30
  }
  printf("\n%d\n", (int)(time(NULL) - t));      // T=TICK():?:?T/60;".";T/6%60
  //--

  Keyboard__(&kbd);
  return 0;
}
