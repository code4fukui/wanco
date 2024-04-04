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
