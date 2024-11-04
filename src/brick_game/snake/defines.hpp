// #ifndef DEFINES_H
// #define DEFINES_H

// #define WIN_INIT(time)    \
//   {                       \
//     initscr();            \
//     noecho();             \
//     curs_set(0);          \
//     keypad(stdscr, TRUE); \
//     timeout(time);        \
//   }

// #define GET_USER_INPUT getch()

// #define MVPRINTW(y, x, ...) \
//   mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
// #define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
// #define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')



// #define ROWS_MAP 20
// #define COLS_MAP 10
// #define SCORE_FOR_NXT_LVL 5
// #define MAX_LVL 10
// #define BASE_DELAY 400
// #define SCORE_FILE "brick_game/tetris/score.txt"
// #define INITIAL_TIMEOUT 40

// #define NOSIG 8
// #define ESCAPE_KEY 27
// #define ENTER_KEY 10
// #define R_KEY 35
// #define P_KEY 112
// #define P_KEY_SMALL 80

// #define BOARDS_BEGIN 2
// #define SNAKE_START_X (10)
// #define SNAKE_START_Y (5)
// #define INTRO_MESSAGE_LEN 21
// #define BOARD_N (ROWS_MAP + MAP_PADDING * 2)
// #define BOARD_M 30
// #define HUD_WIDTH 12
// #define MAP_PADDING 3
// #define INTRO_MESSAGE "Press ENTER to Start!"
// #define BLOCK "■"

// #define SUCCESS 0

// #endif