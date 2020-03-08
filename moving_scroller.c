#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void usage(char* argv[])
{
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage(argv);
        exit(1);
    }

    WINDOW* lister;
    FILE* fp;
    char c;

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    lister = newwin(getmaxy(stdscr), getmaxx(stdscr) / 2, 0, getmaxx(stdscr) / 4);
    wbkgd(lister, COLOR_PAIR(1) | ' ');
    wattron(lister, A_BOLD);
    immedok(lister, TRUE);
    keypad(lister, TRUE);
    scrollok(lister, TRUE);

    fp = fopen(argv[1], "r");
    while ((c = fgetc(fp)) != EOF) {
        waddch(lister, c);
        napms(1);
    }
    fclose(fp);
    
    while (true) {
        int c = wgetch(lister);
        int y, x;
        getbegyx(lister, y, x);
        switch(c){
            case KEY_LEFT:
                x--;
                break;
            case KEY_UP:
                y--;
                break;
            case KEY_RIGHT:
                x++;
                break;
            case KEY_DOWN:
                y++;
                break;
            case 'u':
            case 'U':
                wscrl(lister, 1);
                break;
            case 'd':
            case 'D':
                wscrl(lister, -1);
                break;
        }
        mvwin(lister, y, x);
        touchwin(stdscr);
        refresh();
        wrefresh(lister);
    }

    endwin();
    return 0;
}
