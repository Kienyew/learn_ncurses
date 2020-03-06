
#include <ncurses.h>

int main() {
    WINDOW* cusscr;
    initscr();

    cusscr = newwin(0, 0, 0, 0);
    waddstr(cusscr, "new window created!\n");
    wrefresh(cusscr);
    wgetch(cusscr);

    endwin();
    return 0;
}
