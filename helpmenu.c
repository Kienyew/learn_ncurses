#include <ncurses.h>

WINDOW* help_win;

void showhelp()
{
    touchwin(help_win);
    wrefresh(help_win);

    getch();
    touchwin(stdscr);
    refresh();
}

void wbackspace(WINDOW* win) {
    int y, x, maxx, maxy;
    getyx(win, y, x);
    if (x == 0) {
        y--;
        getmaxyx(win, maxy, maxx);
        x = maxx - 1;
    } else  {
        x--;
    }

    mvwdelch(win, y, x);
}

int main() {
    int c;
    initscr();
    keypad(stdscr, TRUE);
    help_win = newwin(0, 0, 0, 0);
    mvwaddstr(help_win, 6, 32, "Help menu sreen");
    mvwaddstr(help_win, 9, 28, "Press ~ key to quit");
    mvwaddstr(help_win, 12, 28, "Press enter to go back");

    addstr("Typer Program\n");
    addstr("Press + for help\n\n");
    refresh();
    noecho();
    while ((c = getch()) != '~') {
        if (c == '+') {
            showhelp();
        } else if (c == KEY_BACKSPACE || c == '\b' || c == 127) {
            wbackspace(stdscr);
            refresh();
        } else {
            addch(c);
            refresh();
        }
    }

    endwin();
    return 0;
}
