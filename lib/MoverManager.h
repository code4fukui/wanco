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
