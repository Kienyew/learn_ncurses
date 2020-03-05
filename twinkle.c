#include <ncurses.h>

int main() {
    initscr();
    
    attron(A_BOLD);
    addstr("Twinkle, twinkle little star\n");
    attron(A_BLINK);
    addstr("how i wonder what you are\n");
    attroff(A_BLINK);
    addstr("up above the world so high\n");
    addstr("like a diamond in the sky\n");
    attrset(A_NORMAL);
    addstr("twinkle twinkle little star\n");
    addstr("how i wonder what you are\n");
    refresh();
    getch();

    endwin();
    return 0;
}
