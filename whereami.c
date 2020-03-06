#include <ncurses.h>
#include <stdbool.h>

int main() {
    char c;
    initscr();
    
    printw("type: ");
    while (true) {
        int y, x;
        getyx(stdscr, y, x);
        mvprintw(LINES - 1, 0, " %d:%d ", y, x);
        move(y, x);
        c = getch();
    }

    endwin();
    return 0;
}
