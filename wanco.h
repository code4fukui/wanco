#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

#define SCR_W 96
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

#include "wanco_res.h"

// Game objects
typedef struct House House;
typedef struct Meter Meter;

Screen        g_scr;
Keyboard      g_kbd;
ButtonManager g_btm;
MoverManager  g_mvm;

Sprite g_gnd;
House g_house;
House g_housex;
Meter g_meterx;



// Game classes
//---
typedef struct Wanco{
    Sprite base;
    int sx_milli;
    int op;
} Wanco;

int Wanco_isGhostMoveFinished(Mover* yura){
    if( yura->target->y_milli + yura->target->img->h*1000 < 0 ){
        free( Sprite__( Screen_remSprite(&g_scr, yura->target)) );
        free( MoverYura__((MoverYura*)yura) );
        return 1;
    }

    return 0;
}

void Wanco_tick(Sprite* p){
    p->x_milli += ((Wanco*)p)->sx_milli;
    if(p->x_milli < ((Sprite*)&g_housex)->x_milli+((Sprite*)&g_housex)->img->w*1000){
        House* House_attacked(House* p, int op);//###
        House_attacked(&g_housex, ((Wanco*)p)->op);

        Sprite* ghost = Sprite_( malloc(sizeof(Sprite)), &resGhost, Sprite_tick_dummy);{
            ghost->x_milli = p->x_milli + p->img->w*1000/2 - ghost->img->w*1000/2;
            ghost->y_milli = p->y_milli + p->img->h*1000/2 - ghost->img->h*1000/2;
        }
        Screen_addSprite(&g_scr, ghost);

        MoverYura* yura = MoverYura_( malloc(sizeof(MoverYura)), (Sprite*)ghost, Wanco_isGhostMoveFinished);{
            yura->sx_milli = 0;
            yura->sy_milli = -900;
        }
        MoverManager_addMover(&g_mvm, (Mover*)yura);

        free( Sprite__(Screen_remSprite(&g_scr, p)) );//### move to near malloc
    }
}

Wanco* Wanco_(Wanco* p, Image* pWanco){
    Sprite_(&p->base, pWanco, Wanco_tick);
    p->sx_milli = 0;
    p->op = 0;
    return p;
}

Wanco* Wanco__(Wanco* p){
    Sprite__(&p->base);
    return p;
}

//---
typedef struct Meter{
    Sprite base;
    Image img;
} Meter;

Meter* Meter_(Meter* p){
    p->img = resMeter;
    Sprite_(&p->base, &p->img, Sprite_tick_dummy);
    return p;
}

Meter* Meter__(Meter* p){
    Sprite__(&p->base);
    return p;
}

void Meter_set(Meter* p, int percent){
    int i = 9;
    while(percent/10 <= i){
        p->img.buf[1+i] = '_';
        --i;
    }
    while(0 <= i){
        p->img.buf[1+i] = '#';
        --i;
    }
}

//---
typedef struct House{
    Sprite base;
    int hp_max;
    int hp;
} House;

House* House_(House* p, Image* pHouse){
    Sprite_(&p->base, pHouse, Sprite_tick_dummy);
    p->hp = INT32_MAX;
    return p;
}

House* House__(House* p){
    Sprite__(&p->base);
    return p;
}

int House_isMoveFinished(Mover* fall){
    if( g_gnd.y_milli-(4*1000) < fall->target->y_milli ){
        free( MoverFall__((MoverFall*)fall) );
        return 1;
    }

    return 0;
}

House* House_attacked(House* p, int op){
    if( 0 < p->hp){
        if( 0 < (p->hp -= op) ){
            // none
        }else{
            p->hp = 0;

            MoverFall* fall = MoverFall_( malloc(sizeof(MoverFall)), (Sprite*)p, House_isMoveFinished);{
                fall->sx_milli = -120;
                fall->sy_milli = -600;
            }
            MoverManager_addMover(&g_mvm, (Mover*)fall);
        }
        Meter_set(&g_meterx, p->hp*100/p->hp_max);
    }else{
        // none
    }
    return p;
}



// Game events
void onWanco(){
    Wanco* wanco = Wanco_( malloc(sizeof(Wanco)), &resWanco );{
        ((Sprite*)wanco)->x_milli = ((Sprite*)&g_house)->x_milli - ((Sprite*)wanco)->img->w/2*1000;
        ((Sprite*)wanco)->y_milli = g_gnd.y_milli - ((Sprite*)wanco)->img->h*1000;
        wanco->sx_milli = -1000;
        wanco->op = 2;
    }
    Screen_addSprite(&g_scr, (Sprite*)wanco);
    write(STDOUT_FILENO, "\a", 1);
}

void onNoppo(){
    Wanco* wanco = Wanco_( malloc(sizeof(Wanco)), &resNoppo );{
        ((Sprite*)wanco)->x_milli = ((Sprite*)&g_house)->x_milli - ((Sprite*)wanco)->img->w/2*1000;
        ((Sprite*)wanco)->y_milli = g_gnd.y_milli - ((Sprite*)wanco)->img->h*1000;
        wanco->sx_milli = -300;
        wanco->op = 5;
    }
    Screen_addSprite(&g_scr, (Sprite*)wanco);
    write(STDOUT_FILENO, "\a", 1);
}
