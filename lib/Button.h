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
