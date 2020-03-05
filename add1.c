#include <ncurses.h>

int main()
{
    initscr();
    char text[] = "Greetings from NCurses!";
    char* t = text;
    while (*t != '\0') {
        addch(*t++);
        refresh();
        napms(100);
    }
    getch(); /* wait */
    endwin();
    return 0;
}
