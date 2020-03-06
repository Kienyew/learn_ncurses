#include <ncurses.h>
#include <ctype.h>
#include <stdbool.h>

int rot13(int ch) {
    if (!isalpha(ch))
        return ch;

    if (tolower(ch) < 'n')
        return ch + 13;
    else
        return ch - 13;
}

int main()
{
    initscr();
    noecho();
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    WINDOW* left_win = newwin(LINES, COLS / 2,  0, 0);
    WINDOW* right_win = newwin(LINES, COLS / 2, 0, COLS / 2);
   
    int ch;
    while (true) {
        ch = wgetch(left_win);
        waddch(left_win, ch);
        waddch(right_win, rot13(ch));
        wrefresh(left_win);
        wrefresh(right_win);
    }

    endwin();
    return 0;
}
