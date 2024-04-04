#include "wanco.h"

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


