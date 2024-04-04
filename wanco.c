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



// Image
typedef struct Image{
    int w;
    int h;
    char buf[IMG_MAX_W*IMG_MAX_H];
} Image;



// Sprite
typedef struct Sprite{
    Image* img;
    int x_milli;
    int y_milli;
    void (*tick)(struct Sprite*);
} Sprite;

void Sprite_tick_dummy(Sprite* p){}

Sprite* Sprite_(Sprite* p, Image* img, void (*tick)(Sprite*)){
    p->img = img;
    p->x_milli = 0;
    p->y_milli = 0;
    p->tick = tick;
    return p;
}

Sprite* Sprite__(Sprite* p){
    return p;
}

char Sprite_getChar(Sprite* p, int x, int y){
    char r = 0;

    int myx = x - p->x_milli/1000;
    int myy = y - p->y_milli/1000;
    if(
        (0 <= myx && myx < p->img->w) &&
        (0 <= myy && myy < p->img->h) &&
    1){
        r = p->img->buf[p->img->w*myy+myx];
    }

    return r;
}

// Button
typedef struct Button{
    Sprite base;
    Image* img0;
    Image* img1;
    void (*press)(struct Button*);
} Button;

void Button_dummy(){}

Button* Button_(Button* p, Image* img0, Image* img1, void (*press)(Button*)){
    Sprite_(&p->base, img0, Sprite_tick_dummy);
    p->img0 = img0;
    p->img1 = img1;
    p->press = press;
    return p;
}

Button* Button__(Button* p){
    Sprite__(&p->base);
    return p;
}

Button* Button_setActive(Button* p, int act){
    p->base.img = act ? p->img1 : p->img0;
    return p;
}

Button* Button_press(Button* p){
    p->press(p);
    return p;
}



// Mover
typedef struct Mover{
    Sprite* target;
    void (*tick)(struct Mover*);
    int (*isFinished)(struct Mover*);
} Mover;

void Mover_tick_dummy(Mover* p){}
int Mover_isFinished_dummy(Mover* p){ return 0; }

Mover* Mover_(Mover* p, Sprite* target, void (*tick)(Mover*), int (*isFinished)(Mover*)){
    p->target = target;
    p->tick = tick;
    p->isFinished = isFinished;
    return p;
}

Mover* Mover__(Mover* p){
    return p;
}

int Mover_tick(Mover* p){
    p->tick(p);
    return p->isFinished(p);
}

//_
typedef struct MoverFall{
    Mover base;
    int sx_milli;
    int sy_milli;
} MoverFall;

void MoverFall_tick(Mover* p){
    ((MoverFall*)p)->sy_milli += 100;
    p->target->x_milli += ((MoverFall*)p)->sx_milli;
    p->target->y_milli += ((MoverFall*)p)->sy_milli;
}

MoverFall* MoverFall_(MoverFall* p, Sprite* target, int (*isFinished)(Mover*)){
    Mover_(&p->base, target, MoverFall_tick, isFinished);
    p->sx_milli = 0;
    p->sy_milli = 0;
    return p;
}

MoverFall* MoverFall__(MoverFall* p){
    Mover__(&p->base);
    return p;
}

//_
typedef struct MoverYura{
    Mover base;
    int sx_milli;
    int sy_milli;
} MoverYura;

void MoverYura_tick(Mover* p){
    p->target->x_milli += ((MoverYura*)p)->sx_milli;
    p->target->y_milli += ((MoverYura*)p)->sy_milli;
}

MoverYura* MoverYura_(MoverYura* p, Sprite* target, int (*isFinished)(Mover*)){
    Mover_(&p->base, target, MoverYura_tick, isFinished);
    p->sx_milli = 0;
    p->sy_milli = 0;
    return p;
}

MoverYura* MoverYura__(MoverYura* p){
    Mover__(&p->base);
    return p;
}



// Screen
typedef struct Screen{
    char buf[SCR_W*SCR_H];
    Sprite* sprs[SCR_MAX_SPR];
    int     sprs_n;
} Screen;

Screen* Screen_(Screen* p){
    write(STDOUT_FILENO, "\e[?1049h", 8);

    int i=0;
    while(i<SCR_W*SCR_H){
        p->buf[i] = SCR_BG;
        ++i;
    }

    p->sprs_n = 0;

    return p;
}

Screen* Screen__(Screen* p){
    write(STDOUT_FILENO, "\e[?1049l", 8);
    return p;
}

char Screen_set(Screen* p, int x, int y, char c){
    char r = -1;

    unsigned pos = SCR_W*y+x;
    if(pos < sizeof(p->buf)){
        r = p->buf[pos];
        p->buf[pos] = c;
    }

    return r;
}

Sprite* Screen_addSprite(Screen* p, Sprite* s){
    p->sprs[p->sprs_n++] = s;
    return s;
}

Sprite* Screen_remSprite(Screen* p, Sprite* s){
    int i = 0;
    while(i<p->sprs_n && p->sprs[i]!=s) ++i;

    while((i+1) < p->sprs_n){
        p->sprs[i] = p->sprs[i+1];
        ++i;
    }
    --p->sprs_n;

    return s;
}

Screen* Screen_draw(Screen* p){
    write(STDOUT_FILENO, "\e[2J\e[H", 7);
    int y=0;
    while(y<SCR_H){
        int x=0;
        while(x<SCR_W){
            int i = p->sprs_n;
            char c=0;
            while( 0<=--i && !(c=Sprite_getChar(p->sprs[i], x, y)) );

            write(STDOUT_FILENO, c ? &c : &p->buf[SCR_W*y+x], 1);
            ++x;
        }
        write(STDOUT_FILENO, "\n", 1);
        ++y;
    }
    return p;
}

Screen* Screen_tick(Screen* p){
    int i = p->sprs_n;
    while( 0<=--i ) p->sprs[i]->tick(p->sprs[i]);

    return p;
}



// Keyboard
typedef struct Keyboard{
    // none
} Keyboard;

Keyboard* Keyboard_(Keyboard* p){
    struct termios ti;
    tcgetattr( STDIN_FILENO, &ti);
    ti.c_lflag &= ~(ICANON);
    tcsetattr( STDIN_FILENO, TCSANOW, &ti);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    return p;
}

Keyboard* Keyboard__(Keyboard* p){
    return p;
}

char Keyboard_inkey(Keyboard* p){
    char c;
    return (read(STDIN_FILENO, &c, 1) == 1)
        ? (c == '\e')
            ? (read(STDIN_FILENO, &c, 1) == 1)
                ? (c == '[')
                    ? (read(STDIN_FILENO, &c, 1) == 1)
                        ? c //###
                        : 0 //###
                    : 0 //###
                : '\e'
            : c
        : 0;
}



// Button manager
typedef struct ButtonManager{
    Button* btns[BTM_MAX_BTN];
    int     btns_n;
    int     cur;
    Button  sentry;
} ButtonManager;

ButtonManager* ButtonManager_(ButtonManager* p){
    p->cur = 0;
    p->btns[0] = Button_(&p->sentry, 0, 0, Button_dummy);
    p->btns_n = 1;
    return p;
}

ButtonManager* ButtonManager__(ButtonManager* p){
    return p;
}

Button* ButtonManager_addButton(ButtonManager* p, Button* b){
    p->btns[p->btns_n++] = b;
    return b;
}

void ButtonManager_activeNext(ButtonManager* p){
    Button_setActive(p->btns[p->cur], 0);
    p->cur = (++p->cur) % p->btns_n;
    Button_setActive(p->btns[p->cur], 1);
}

void ButtonManager_runActive(ButtonManager* p){
    Button_press(p->btns[p->cur]);
}



// Mover manager
typedef struct MoverManager{
    Mover* mvrs[MVM_MAX_MVR];
    int    mvrs_n;
} MoverManager;

Mover* MoverManager_addMover(MoverManager* p, Mover* m){
    p->mvrs[p->mvrs_n++] = m;
    return m;
}

Mover* MoverManager_remMover(MoverManager* p, Mover* m){
    int i = 0;
    while(i<p->mvrs_n && p->mvrs[i]!=m) ++i;

    while((i+1) < p->mvrs_n){
        p->mvrs[i] = p->mvrs[i+1];
        ++i;
    }
    --p->mvrs_n;

    return m;
}

MoverManager* MoverManager_tick(MoverManager* p){
    int i = p->mvrs_n;
    while( 0<=--i ){
        if( Mover_tick(p->mvrs[i]) ){
            MoverManager_remMover(p, p->mvrs[i]);
        }
    }

    return p;
}



// Game resouces
Image resBtnWanco0 = {
    .w   = 7,
    .h   = 3,
    .buf = "-------"
           " wanco "
           "-------"
};
Image resBtnWanco1 = {
    .w   = 7,
    .h   = 3,
    .buf = "======="
           "|wanco|"
           "======="
};

Image resBtnNoppo0 = {
    .w   = 7,
    .h   = 3,
    .buf = "-------"
           " noppo "
           "-------"
};
Image resBtnNoppo1 = {
    .w   = 7,
    .h   = 3,
    .buf = "======="
           "|noppo|"
           "======="
};

Image resGhost = {
    .w   = 4,
    .h   = 3,
    .buf = " __ "
           "('')"
           " || "
};

Image resWanco = {
    .w   = 8,
    .h   = 4,
    .buf = "\0/----\\\0"
           "| O  O |"
           "|  ==  |"
           "\0\\----/\0"
};

Image resNoppo = {
    .w   = 6,
    .h   = 10,
    .buf = "\0/--\\\0"
           "|O  O|"
           "|    |"
           "| == |"
           "|    |"
           "|    |"
           "|    |"
           "|    |"
           "|    |"
           "\0\\--/\0"
};

Image resHouse = {
    .w   = 5,
    .h   = 8,
    .buf = "\0\0_\0\0"
           "\0/ \\\0"
           "/ _ \\"
           "| + |"
           "| = |"
           "| _ |"
           "|! !|"
           "||_||"
};

Image resHouseX = {
    .w   = 5,
    .h   = 8,
    .buf = "\0\0_\0\0"
           "\0/ \\\0"
           "/ _ \\"
           "| x |"
           "| = |"
           "| _ |"
           "|! !|"
           "||_||"
};

Image resMeter = {
    .w   = 12,
    .h   = 1,
    .buf = "[##########]"
};

Image resGround = {
    .w   = 96,
    .h   = 8,
    .buf = "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
};



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



// Game process
int main(){
    Screen_(&g_scr);
    Keyboard_(&g_kbd);
    ButtonManager_(&g_btm);

    // init something
    Sprite_(&g_gnd, &resGround, Sprite_tick_dummy);
    g_gnd.x_milli = (0)*1000;
    g_gnd.y_milli = (SCR_H-g_gnd.img->h)*1000;

    House_(&g_house, &resHouse);
    ((Sprite*)&g_house)->x_milli = (SCR_W-3-((Sprite*)&g_house)->img->w)*1000;
    ((Sprite*)&g_house)->y_milli = g_gnd.y_milli - (((Sprite*)&g_house)->img->h)*1000;

    House_(&g_housex, &resHouseX);
    ((Sprite*)&g_housex)->x_milli = (3)*1000;
    ((Sprite*)&g_housex)->y_milli = g_gnd.y_milli - (((Sprite*)&g_housex)->img->h)*1000;
    g_housex.hp_max = g_housex.hp = 10;

    Meter_(&g_meterx);
    ((Sprite*)&g_meterx)->x_milli = (1)*1000;
    ((Sprite*)&g_meterx)->y_milli = (1)*1000;

    Button btnNoppo;
    Button_(&btnNoppo, &resBtnNoppo0, &resBtnNoppo1, onNoppo);
    ((Sprite*)&btnNoppo)->x_milli = (SCR_W-2-btnNoppo.img0->w)*1000;
    ((Sprite*)&btnNoppo)->y_milli = (SCR_H-2-btnNoppo.img0->h)*1000;

    Button btnWanco;
    Button_(&btnWanco, &resBtnWanco0, &resBtnWanco1, onWanco);
    ((Sprite*)&btnWanco)->x_milli = ((Sprite*)&btnNoppo)->x_milli - (2+btnWanco.img0->w)*1000;
    ((Sprite*)&btnWanco)->y_milli = (SCR_H-2-btnWanco.img0->h)*1000;

    Screen_addSprite(&g_scr, (Sprite*)&g_house);
    Screen_addSprite(&g_scr, (Sprite*)&g_housex);
    Screen_addSprite(&g_scr, (Sprite*)&g_meterx);
    Screen_addSprite(&g_scr, &g_gnd);
    Screen_addSprite(&g_scr, (Sprite*)&btnWanco);
    Screen_addSprite(&g_scr, (Sprite*)&btnNoppo);

    ButtonManager_addButton(&g_btm, &btnWanco);
    ButtonManager_addButton(&g_btm, &btnNoppo);
    //---

    char c=0;
    while(!( c=='\e' )){
        Screen_draw(&g_scr);
        usleep(50000);

        MoverManager_tick(&g_mvm);
        Screen_tick(&g_scr);

        c = Keyboard_inkey(&g_kbd);
        if(c == ' '){
            ButtonManager_runActive(&g_btm);
        }else
        if(c == '\t'){
            ButtonManager_activeNext(&g_btm);
        }
        //---
    }

    ButtonManager__(&g_btm);
    Keyboard__(&g_kbd);
    Screen__(&g_scr);

    return 0;
}


