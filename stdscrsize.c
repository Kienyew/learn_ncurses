#include <ncurses.h>
#include <stdbool.h>

int main() {
    int row, col;
    initscr();
     
    while (true) {
        getmaxyx(stdscr, row, col);
        printw("row: %d\ncol: %d\nLINES: %d\nCOLS: %d\n\n", row, col, LINES, COLS);
        refresh();
        napms(1000);
    }

    endwin();
    return 0;
}
