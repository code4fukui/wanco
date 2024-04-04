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
