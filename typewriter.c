#include <ncurses.h>
#include <stdlib.h>     /* srand(), rand() */ 
#include <time.h>       /* time() */

#define RANDINT(min, max) ((min) + rand() % (max))




int main() {
    srand(time(NULL));
    char c;

    initscr();
    addstr("2020-03-05 20:40:19 > Typewriter program with delayed output\n");
    addstr("Press ~ to quit\n\n");
    refresh();
    while ((c = getch()) != '~')
        napms(RANDINT(50,75));

    endwin();
    return 0;
}
