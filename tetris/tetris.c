#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>
#include <string.h> /* memcpy */

#include "ttime.h"

/* GAME CONCEPTS
 * =============
 * 2 windows - gamewin & statwin
 * 
 * STRUCTURES
 * ----------
 *  TetrisCell - storing information of each cell on grid.
 * TetrisShape - storing information of the falling shape.
 * 
 *
 * 
 * ON GAME START
 * -------------
 * initialize curses
 * initialize windows
 * initialize tetrisboard
 * 
 * ON GAME UPDATE (mainloop)
 * -------------------------
 * if no active shape:
 *     add one on top
 * 
 * if timeout
 *     push curshape down one row
 * 
 * receive & process input:
 *     KEY_UP - rotate
 *     KEY_LEFT or KEY_RIGHT - move
 *     KEY_DOWN - push down
 * 
 * 
 * 
 * 
 */

#define RANDINT(min, max) ((min) + rand() % (max - min + 1))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define SHAPE_COUNT 7

enum Rotation { ROT_UP = 0, ROT_LEFT, ROT_DOWN, ROT_RIGHT };

struct TetrisCell {
    bool occupied;
    int color_pair;
};

struct Shape {
    struct ShapeStruct const* struc;
    int color_pair;
    int x;
    int y;
    enum Rotation rot;
};

struct ShapeStruct {
    int n;
    bool _interstruc[4][4][4];
};

/* GLOBAL VARIABLES */
int BOARD_COLS = 0;
int BOARD_ROWS = 0;
int DROP_SPEED_MS = 0;

struct timespec gamestarttime;

WINDOW* gamewin;
WINDOW* statwin;
struct TetrisCell** tetrisboard;
struct Shape* curshape = NULL;
struct Shape _curshape = { 0 };
time_t nextforcedroptime = 0;
bool gameover = false;

const struct ShapeStruct T_SHAPE_STRUCT_O = {
    .n = 2,
    ._interstruc = {
        {{1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        {{1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        {{1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        {{1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
    }
};

const struct ShapeStruct T_SHAPE_STRUCT_T = {
    .n = 3,
    ._interstruc = {
        {{0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        {{0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
        {{0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
        {{0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
    }
};

const struct ShapeStruct T_SHAPE_STRUCT_L1 = {
    .n = 3,
    ._interstruc = {
        {{1,0,0,0},
         {1,0,0,0},
         {1,1,0,0},
         {0,0,0,0}},
        {{0,0,1,0},
         {1,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},
        {{1,1,0,0},
         {0,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},
        {{1,1,1,0},
         {0,0,1,0},
         {0,0,0,0},
         {0,0,0,0}},
    }
};

const struct ShapeStruct T_SHAPE_STRUCT_L2 = {
    .n = 3,
    ._interstruc = {
        {{0,1,0,0},
         {0,1,0,0},
         {1,1,0,0},
         {0,0,0,0}},
        {{1,1,1,0},
         {0,0,1,0},
         {0,0,0,0},
         {0,0,0,0}},
        {{1,1,0,0},
         {1,0,0,0},
         {1,0,0,0},
         {0,0,0,0}},
        {{1,0,0,0},
         {1,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},
    }
};

const struct ShapeStruct T_SHAPE_STRUCT_Z1 = {
    .n = 3,
    ._interstruc = {
        {{1,0,0,0},
         {1,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},
        {{0,1,1,0},
         {1,1,0,0},
         {0,0,0,0},
         {0,0,0,0}},
        {{1,0,0,0},
         {1,1,0,0},
         {0,1,0,0},
         {0,0,0,0}},
        {{0,1,1,0},
         {1,1,0,0},
         {0,0,0,0},
         {0,0,0,0}},
    }
};

const struct ShapeStruct T_SHAPE_STRUCT_Z2 = {
    .n = 3,
    ._interstruc = {
        {{0,1,0,0},
         {1,1,0,0},
         {1,0,0,0},
         {0,0,0,0}},
        {{1,1,0,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},
        {{0,1,0,0},
         {1,1,0,0},
         {1,0,0,0},
         {0,0,0,0}},
        {{1,1,0,0},
         {0,1,1,0},
         {0,0,0,0},
         {0,0,0,0}},
    }
};

const struct ShapeStruct T_SHAPE_STRUCT_I = {
    .n = 4,
    ._interstruc = {
        {{0,1,0,0},
         {0,1,0,0},
         {0,1,0,0},
         {0,1,0,0}},
        {{0,0,0,0},
         {1,1,1,1},
         {0,0,0,0},
         {0,0,0,0}},
        {{0,1,0,0},
         {0,1,0,0},
         {0,1,0,0},
         {0,1,0,0}},
        {{0,0,0,0},
         {1,1,1,1},
         {0,0,0,0},
         {0,0,0,0}},
    }
};

const struct ShapeStruct* t_shape_structures[] = {
    &T_SHAPE_STRUCT_O,
    &T_SHAPE_STRUCT_T,
    &T_SHAPE_STRUCT_L1,
    &T_SHAPE_STRUCT_L2,
    &T_SHAPE_STRUCT_Z1,
    &T_SHAPE_STRUCT_Z2,
    &T_SHAPE_STRUCT_I
};


void parse_args(int argc, char* argv[])
{
    static char* usage_str = "Usage: %s [-h, --help] [--cols COLS] [--lines LINES] [--drop-speed MILISECONDS]\n";
    int err;
    for (int i = 1; i < argc; ++i) {
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            fprintf(stderr, usage_str, argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "--cols") == 0) {
            int _cols;
            if (i + 1 >= argc)
                goto invalid_option;

            err = sscanf(argv[++i], "%d", &_cols);
            if (err == 0)
                goto invalid_option;

            BOARD_COLS = _cols;
        } else if (strcmp(argv[i], "--lines") == 0) {
            int _lines;
            if (i + 1 >= argc)
                goto invalid_option;

            err = sscanf(argv[++i], "%d", &_lines);
            if (err == 0)
                goto invalid_option;

            BOARD_ROWS = _lines;
        } else if (strcmp(argv[i], "--drop-speed") == 0) {
            int _speed;
            if (i + 1 >= argc)
                goto invalid_option;

            err = sscanf(argv[++i], "%d", &_speed);
            if (err == 0)
                goto invalid_option;

            DROP_SPEED_MS = _speed;
        } else {
            goto invalid_option;
        }
    }

    return;

    invalid_option:
    fprintf(stderr, usage_str, argv[0]);
    exit(1);

}

bool shape_struct_filled_at(const struct ShapeStruct* struc, enum Rotation rot, int row, int col)
{
    return struc->_interstruc[(int)rot][row][col];
}

void init_windows()
{
    gamewin = subwin(stdscr, BOARD_ROWS, BOARD_COLS, 1, 1);
    statwin = subwin(stdscr, BOARD_ROWS, getmaxx(stdscr) - BOARD_COLS - 3, 1, 2 + BOARD_COLS);
    refresh();
}

void tetris_curshape_solidify()
{
    assert(curshape != NULL);
    for (int row = 0; row < curshape->struc->n; ++row)
    for (int col = 0; col < curshape->struc->n; ++col) {
        if (shape_struct_filled_at(curshape->struc, curshape->rot, row, col)) {
            assert(!tetrisboard[curshape->y + row][curshape->x + col].occupied);
            tetrisboard[curshape->y + row][curshape->x + col].occupied = true;
            tetrisboard[curshape->y + row][curshape->x + col].color_pair = curshape->color_pair;
        }
    }
    curshape = NULL;
}


void tetris_curshape_pushdown() 
{
    assert(curshape != NULL);
    nextforcedroptime = getgametime_ms() + DROP_SPEED_MS;

    /* check is reach bottom */
    for (int row = 0; row < curshape->struc->n; ++row)
    for (int col = 0; col < curshape->struc->n; ++col) {
        if (shape_struct_filled_at(curshape->struc, curshape->rot, row, col)) {
            if (curshape->y + row + 1 == BOARD_ROWS) {
                tetris_curshape_solidify();
                return;
            }
        }
    }

    /* check if next row has block */
    for (int row = 0; row < curshape->struc->n; ++row)
    for (int col = 0; col < curshape->struc->n; ++col) {
        if (shape_struct_filled_at(curshape->struc, curshape->rot, row, col)) {
            if (tetrisboard[curshape->y + row + 1][curshape->x + col].occupied) {
                tetris_curshape_solidify();
                return;
            }
        }
    }


    /* all is fine, go to next row */
    curshape->y ++;
}

bool tetris_curshape_move_lateral(int dx)
{
    assert(curshape != NULL);
    for (int row = 0; row < curshape->struc->n; ++row)
    for (int col = 0; col < curshape->struc->n; ++col) {
        if (shape_struct_filled_at(curshape->struc, curshape->rot, row, col)) {
            if (curshape->x + col + dx < 0 || curshape->x + col + dx >= BOARD_COLS)
                return false;

            if (tetrisboard[curshape->y + row][curshape->x + col + dx].occupied)
                return false;
        }
    }

    curshape->x += dx;
    return true;
}

bool tetris_curshape_rotate(enum Rotation rot)
{
    assert(curshape != NULL);

    for (int row = 0; row < curshape->struc->n; ++row)
    for (int col = 0; col < curshape->struc->n; ++col) {
        if (shape_struct_filled_at(curshape->struc, rot, row, col)) {
            if (curshape->x + col < 0 || curshape ->x + col >= BOARD_COLS)
                return false;
            if (curshape->y + row < 0 || curshape->y + row >= BOARD_ROWS)
                return false;
            if (tetrisboard[curshape->y + row][curshape->x + col].occupied)
                return false;
        }
    }

    curshape->rot = rot;
    return true;
}

void tetris_remove_line(int row)
{
    assert(row > 0 && row < BOARD_ROWS);
    for (int r = row; r > 0; --r) {
        memcpy((void*)&tetrisboard[r], (void*)&tetrisboard[r - 1], sizeof(tetrisboard[0]));
    }
    
    for (int col = 0; col < BOARD_COLS; ++col) {
        tetrisboard[0][col].occupied = false;
    }
}

void update_game()
{
    /* refresh game stat */
    gameover = false;

    if (curshape == NULL) {
        const struct ShapeStruct* struc = t_shape_structures[RANDINT(0, SHAPE_COUNT - 1)];
        _curshape.struc = struc;
        _curshape.color_pair = RANDINT(1, 6);
        _curshape.y = 0;
        _curshape.x = RANDINT(0, BOARD_COLS - struc->n);
        _curshape.rot = ROT_UP;
        curshape = &_curshape;

        for (int row = 0; row < curshape->struc->n; ++row)
        for (int col = 0; col < curshape->struc->n; ++col) {
            if (shape_struct_filled_at(curshape->struc, curshape->rot, row, col))
            if (tetrisboard[curshape->y + row][curshape->x + col].occupied) {
                gameover = true;
                return;
            }
        }
    }

    assert(curshape != NULL);

    if (getgametime_ms() >= nextforcedroptime) {
        tetris_curshape_pushdown();
    }

    int ch = getch();
    switch(ch) {
        case KEY_LEFT:
            if(curshape)
                tetris_curshape_move_lateral(-1);
            break;
        case KEY_RIGHT:
            if (curshape)
                tetris_curshape_move_lateral(1);
            break;
        case KEY_UP:
            if (curshape)
                tetris_curshape_rotate((curshape->rot + 1) % 4);
            break;
        case KEY_DOWN:
            if (curshape)
                tetris_curshape_pushdown();
            break;
    }

    /* delete completed lines */
    bool* line_complete = (bool*)memset(malloc(sizeof(bool) * BOARD_ROWS), (int)false, sizeof(bool) * BOARD_ROWS);
    for (int row = BOARD_ROWS - 1; row >= 0; --row) {
        int occupied_block_counter = 0;
        for (int col = 0; col < BOARD_COLS; ++col)
            occupied_block_counter += tetrisboard[row][col].occupied;
        
        if (occupied_block_counter == BOARD_COLS)
            line_complete[row] = true;
    }

    for (int row = 0; row < BOARD_ROWS; ++row) {
        if (line_complete[row]) {
            tetris_remove_line(row);
        }
    }
    free(line_complete);
}

void draw_game()
{
    /* borders and lines */
    box(stdscr, 0, 0);
    move(1, 1 + BOARD_COLS);
    vline(0, getmaxy(stdscr) - 2);
    move(1 + BOARD_ROWS, 1);
    hline(0, BOARD_COLS);


    /* tetris board */
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            if (tetrisboard[row][col].occupied)
                wattrset(gamewin, COLOR_PAIR(tetrisboard[row][col].color_pair));
            else 
                wattrset(gamewin, COLOR_PAIR(0));

            mvwaddch(gamewin, row, col, ' ');
        }
    }

    /* active shape */
    if (curshape) {
        for (int row = 0; row < curshape->struc->n; ++row) {
            for (int col = 0; col < curshape->struc->n; ++col) {
                if (shape_struct_filled_at(curshape->struc, curshape->rot, row, col)) {
                    wattrset(gamewin, COLOR_PAIR(curshape->color_pair));
                    mvwaddch(gamewin, curshape->y + row, curshape->x + col, ' ');
                }
            }
        }
    }

    refresh();
    wrefresh(gamewin);
    wrefresh(statwin);
}

int main(int argc, char* argv[]) 
{
    parse_args(argc, argv);
    initscr();
    BOARD_ROWS = (BOARD_ROWS == 0) ? (getmaxy(stdscr) - 2) : BOARD_ROWS;
    BOARD_COLS = (BOARD_COLS == 0) ? (getmaxx(stdscr) / 2 - 2) : BOARD_COLS;
    DROP_SPEED_MS = (DROP_SPEED_MS == 0) ? (275) : DROP_SPEED_MS;
    tetrisboard = (struct TetrisCell**)malloc(sizeof(struct TetrisCell*) * BOARD_ROWS);
    for (int i = 0; i < BOARD_ROWS; ++i) {
        tetrisboard[i] = (struct TetrisCell*)calloc(BOARD_COLS, sizeof(struct TetrisCell));
    }

    start_color();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_CYAN);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_GREEN);

    clock_gettime(CLOCK_REALTIME, &gamestarttime);
    srand(gamestarttime.tv_nsec);

    init_windows();
    while (!gameover) {
        update_game();
        draw_game();
        napms(10);
    }

    nodelay(stdscr, FALSE);
    getch();
    endwin();
    return 0;
}
