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
