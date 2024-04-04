#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

#define SCR_W 80
#define SCR_H 24
#define SCR_BG ' '
#define SCR_MAX_SPR 1024

#define IMG_MAX_W SCR_W
#define IMG_MAX_H SCR_H

#define BTM_MAX_BTN 256
#define MVM_MAX_MVR 256

#include "lib/Image.h"
#include "lib/Sprite.h"
#include "lib/Screen.h"
#include "lib/Mover.h"
#include "lib/Button.h"
#include "lib/Keyboard.h"
#include "lib/ButtonManager.h"
#include "lib/MoverManager.h"
