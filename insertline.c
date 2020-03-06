#include <ncurses.h>

int main() {
    char line1[] = "LINE 1\n";
    char line2[] = "LINE 2\n";
    char line3[] = "LINE 3\n";
    char line4[] = "LINE 4\n";
    char line5[] = "LINE 5\n";
    initscr();

    printw("%s%s%s", line1, line3, line5);
    refresh();
    getch();

    move(1, 0);
    insertln();
    addstr(line2);
    move(3, 0);
    insertln();
    addstr(line4);
    refresh();
    getch();

    endwin();
    return 0;
}
