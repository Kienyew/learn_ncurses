#include <ncurses.h>

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    attrset(COLOR_PAIR(1) | A_BOLD);
    addstr("hey mr yellow\n");

    attrset(COLOR_PAIR(2));
    addstr("i am blue\n");

    refresh();
    getch();
    endwin();
    return 0;
}
