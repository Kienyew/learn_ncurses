#include <ncurses.h>

int main() {
    char first[4];
    char last[4];

    initscr();
    addstr("Enter 3 chars for firstname: ");
    refresh();
    getnstr(first, 3);

    addstr("Enter 3 chars for lastname: ");
    refresh();
    getnstr(last, 3);

    printw("Thanks, %s %s\n", first, last); 
    refresh();
    getch();

    endwin();
    return 0;
}
