#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#define DRAW_WHITE  1
#define DRAW_YELLOW 2
#define DRAW_BLUE   3
#define DRAW_CYAN   4
#define DRAW_RED    5

void init_ncurses()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
    printf("\033[?1003h\n"); // makes the terminal report mouse movement events

    /* colors */
    start_color();
    init_pair(DRAW_WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(DRAW_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(DRAW_BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(DRAW_CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(DRAW_RED, COLOR_RED, COLOR_RED);
}

int main() 
{
    init_ncurses();
    box(stdscr, 0, 0);

    mmask_t mstate = 0;
    while (true) {
        int c = getch();
        if (c == KEY_MOUSE) {
            MEVENT e;
            if (getmouse(&e) != OK)
                continue;

            if (e.bstate & BUTTON1_PRESSED)
                mstate |= BUTTON1_PRESSED;
            
            else if (e.bstate & BUTTON1_RELEASED)
                mstate &= ~BUTTON1_PRESSED;

            else if (e.bstate & BUTTON3_PRESSED)
                mstate |= BUTTON3_PRESSED;

            else if (e.bstate & BUTTON3_RELEASED)
                mstate &= ~BUTTON3_PRESSED;

            else if (e.bstate & BUTTON2_PRESSED)
                mstate |= BUTTON2_PRESSED;

            else if (e.bstate & BUTTON2_RELEASED)
                mstate &= ~BUTTON2_PRESSED;

            /* draw at mouse position */
            if (mstate & (BUTTON1_PRESSED | BUTTON3_PRESSED | BUTTON2_PRESSED)) {
                move(e.y, e.x);
                if (mstate & BUTTON1_PRESSED)
                    attrset(COLOR_PAIR(DRAW_WHITE));

                else if (mstate & BUTTON2_PRESSED & BUTTON_CTRL)
                    attrset(COLOR_PAIR(DRAW_YELLOW));

                else if (mstate & BUTTON2_PRESSED)
                    attrset(COLOR_PAIR(DRAW_BLUE));

                else if (mstate & BUTTON3_PRESSED & BUTTON_CTRL)
                    attrset(COLOR_PAIR(DRAW_RED));

                else if (mstate & BUTTON3_PRESSED)
                    attrset(COLOR_PAIR(DRAW_CYAN));

                addch(' ');
                attrset(COLOR_PAIR(0));
                box(stdscr, 0, 0);
                refresh();

            }
        }
    }
    endwin();
    return 0;
}
