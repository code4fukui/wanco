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
