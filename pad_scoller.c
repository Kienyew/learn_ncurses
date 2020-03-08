#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void usage(char *argv[])
{
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
}

int clamp(int x, int min, int max) {
    return (x < min) ? min : (x > max) ? max : x;
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv);
        exit(1);
    }

    initscr();
    WINDOW* scrollpad = newpad(10000, getmaxx(stdscr) / 2);
    keypad(scrollpad, TRUE);
    prefresh(scrollpad, 0, 0, 0, getmaxx(stdscr) * 1/4, getmaxy(stdscr) - 1, getmaxx(stdscr) * 3/4);
    FILE* fp = fopen(argv[1], "r");
    int c;
    int line_counter = 1;
    wprintw(scrollpad, "%4d|", line_counter++);
    while ((c = fgetc(fp)) != EOF) {
        pechochar(scrollpad, c);
        if (c == '\n') {
            wprintw(scrollpad, "%4d|", line_counter++);
        }
    }


    int scroller_y = 0;
    while (true)
    {
        int c = wgetch(scrollpad);
        switch (c)
        {
            case KEY_UP:
                scroller_y--;
                break;
            case KEY_DOWN:
                scroller_y++;
                break;
        }
        scroller_y = clamp(scroller_y, 0, 9999);
        prefresh(scrollpad, scroller_y, 0, 0, getmaxx(stdscr) * 1/4, getmaxy(stdscr) - 1, getmaxx(stdscr) * 3/4);
    }

    endwin();
    return 0;
}
