#include <ncurses.h>

int main() {
    char text1[] = "yeahh this first line\n";
    char text2[] = "how bout next line";

    initscr();
    addstr(text1);
    addstr(text2);
    move(2, 2);
    addstr("With the Y chromosome changed to the X.");
    refresh();
    getch();

    endwin();
    return 0;
}
