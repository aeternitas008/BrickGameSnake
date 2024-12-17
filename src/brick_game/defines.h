#ifndef DEFINES_H
#define DEFINES_H

#include <array>
#include <deque>

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define GET_USER_INPUT getch()

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define ROWS_MAP 20
#define COLS_MAP 10
#define SCORE_FOR_NXT_LVL_TTR 600
#define SCORE_FOR_NXT_LVL_SNK 5
#define MAX_LVL 10
#define BASE_DELAY_TTR 800
#define BASE_DELAY_SNK 400
#define SCORE_FILE_SNK "scoreSnake.txt"
#define SCORE_FILE_TTR "scoreTetris.txt"
#define INITIAL_TIMEOUT 40

#define NOSIG 8
#define ESCAPE_KEY 27
#define ENTER_KEY 10
#define R_KEY 35
#define P_KEY 112
#define P_KEY_SMALL 80

#define BOARDS_BEGIN 2
#define SNAKE_START_X (10)
#define SNAKE_START_Y (5)
#define TETR_START_X (-1)
#define TETR_START_Y (3)
#define INTRO_MESSAGE_LEN 21
#define BOARD_N (ROWS_MAP + MAP_PADDING * 2)
#define BOARD_M 30
#define HUD_WIDTH 12
#define MAP_PADDING 3
#define INTRO_MESSAGE "Press ENTER to Start!"
#define BLOCK1 "■"
#define BLOCK "#"

#define SUCCESS 0

/**
 * @brief Перечисление действий пользователя.
 */
enum UserAction_t {
  Start = 0,     /**< Начать игру */
  Pause,     /**< Поставить игру на паузу */
  Terminate, /**< Завершить игру */
  Left,      /**< Движение влево */
  Right,     /**< Движение вправо */
  Up,        /**< Движение вверх или поворот */
  Down,      /**< Движение вниз */
  Action,     /**< Выполнение действия */
  Nosig
};

/**
 * @brief Перечисление состояний игры.
 */
enum State_t {
  START = 0, /**< Начало игры */
  SPAWN,     /**< Появление новой фигуры */
  MOVING,    /**< Фигура движется */
  SHIFTING,  /**< Сдвиг или поворот фигуры */
  GAMEOVER,  /**< Конец игры */
  EXIT_STATE /**< Выход из игры */
};

/**
 * @brief Структура, содержащая информацию об игре.
 */
struct GameInfo_t {
  int field[ROWS_MAP][COLS_MAP] = {}; /**< Двумерный массив игрового поля */
  int next[4][4] = {}; /**< Двумерный массив для отображения следующего тетромино */
  int score = 0;  /**< Текущий счет игрока */
  int high_score = 0; /**< Максимальный достигнутый счет */
  int level = 1;      /**< Текущий уровень */
  int speed = 1;      /**< Скорость падения фигур */
  int pause = 0;      /**< Состояние паузы */

  State_t state = START;
};

/**
 * @brief Структура, представляющая координаты на игровом поле.
 */
struct Position_t {
  int x, y; /**< Координаты */
};


#endif