#include <ncurses.h>
#include <stdlib.h>     /* srand(), rand() */ 
#include <time.h>       /* time() */
#include <stdbool.h>

#define RANDINT(min, max) ((min) + rand() % (max))

int main() {
    srand(time(NULL));
    char c;

    initscr();
    addstr("2020-03-06 12:25:21 > Typewriter program with delayed output clear screen support\n");
    addstr("Press ~ to quit\n");
    addstr("Press C to clear screen\n");
    refresh();
    while (true) {
        c = getch();
        if (c == '~') {
            break; 
        } else if (c == 'C') {
            clear();
        }
        napms(RANDINT(50,75));
        refresh();
    }

    endwin();
    return 0;
}
