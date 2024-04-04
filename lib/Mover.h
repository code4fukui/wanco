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
