#include <ncurses.h>
#include <stdio.h>

int main() {
    WINDOW* sonny;
    initscr();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_WHITE);

    sonny = subwin(stdscr, 20, 20, 10, 10);
    wbkgd(stdscr, COLOR_PAIR(1) | ' ');
    wbkgd(sonny, COLOR_PAIR(2) | ' ');
    waddstr(stdscr, "hello child");
    waddstr(sonny, "hi mother");
    refresh();
    getch();



    endwin();
    return 0;
}
