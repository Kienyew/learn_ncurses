#include <ncurses.h>

enum corner_t { UPPER_LEFT, UPPER_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

void cornerchar(enum corner_t corner, char c)
{
    switch(corner) {
        case (UPPER_LEFT):
            move(0, 0);
            break;
        case (UPPER_RIGHT):
            move(0, COLS - 1);
            break;
        case (BOTTOM_LEFT):
            move(LINES - 1, 0);
            break;
        case (BOTTOM_RIGHT):
            move(LINES - 1, COLS - 1);
            break;
    }

    addch(c);
}

int main()
{
    initscr();

    cornerchar(UPPER_LEFT, '*');
    cornerchar(UPPER_RIGHT, '*');
    cornerchar(BOTTOM_LEFT, '*');
    cornerchar(BOTTOM_RIGHT, '*');
    refresh();
    getch();

    endwin();
    return 0;
}

