#include <ncurses.h>

int main() {
    initscr();
    addstr("Attention\n");
    beep();
    refresh();
    getch();


    addstr("ATTENTION\n");
    flash();
    refresh();
    getch();

    endwin();
    return 0;
}
