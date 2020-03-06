#include <ncurses.h>

char buffer[8192];

int main() {
    initscr();

    addstr("type something while i'm sleeping...\n");
    refresh();
    napms(2000);

    getstr(buffer);
    addstr("here's what you have typed: ");
    addstr(buffer);
    refresh();

    napms(1000);
    endwin();
    return 0;
}
