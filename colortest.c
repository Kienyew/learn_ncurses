#include <ncurses.h>

#define STRBOOL(boolean_value) ((boolean_value) ? "true" : "false")

int main() {
    initscr();
    printw("has_colors()? %s\n", STRBOOL(has_colors()));
    printw("start_color()? %s\n", (start_color() == OK) ? "OK" : "NOT OK");
    printw("COLORS == %d\n", COLORS);
    printw("COLOR_PAIRS == %d\n", COLOR_PAIRS);
    refresh();
    getch();

    endwin();
    return 0;
}
