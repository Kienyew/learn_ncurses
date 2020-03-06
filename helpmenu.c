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
    help_win = newwin(10, 30, 4, 26);
    mvwaddstr(help_win, 1, 7, "Help menu sreen");
    mvwaddstr(help_win, 5, 3, "Press ~ key to quit");
    mvwaddstr(help_win, 8, 4, "Press enter to go back");

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

    delwin(help_win);
    endwin();
    return 0;
}
