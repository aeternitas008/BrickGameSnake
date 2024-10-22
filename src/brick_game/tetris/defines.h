#ifndef DEFINES_H
#define DEFINES_H

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

#define SCORE_FILE "brick_game/tetris/score.txt"

#define INTRO_MESSAGE_LEN 21
#define INTRO_MESSAGE "Press ENTER to Start!"

#define MAX_LVL 10
#define BASE_DELAY 1000

#define SCORE_FOR_NXT_LVL 600

#define ROWS_MAP 20
#define COLS_MAP 10

#define BOARDS_BEGIN 2

#define TETRSTART_X (-1)
#define TETRSTART_Y (3)
#define INITIAL_TIMEOUT 50

#define BOARD_N (ROWS_MAP + MAP_PADDING * 2)
#define BOARD_M 30
#define HUD_WIDTH 12
#define MAP_PADDING 3

#define SUCCESS 0

#define NOSIG 8

#define ESCAPE_KEY 27
#define ENTER_KEY 10
#define R_KEY 35
#define P_KEY 112
#define P_KEY_SMALL 80

#define BLOCK "■"

/**
 * @brief Структура, представляющая координаты на игровом поле.
 */
typedef struct {
  int x; /**< Координата X */
  int y; /**< Координата Y */
} Position;

/**
 * @brief Структура, представляющая тетромино, включая его позицию, тип и
 * варианты вращения.
 */
typedef struct {
  Position *point; /**< Указатель на позицию тетромино на игровом поле */
  int type; /**< Тип тетромино (например, I, O, T и т.д.) */
  int variant; /**< Вариант вращения тетромино (0-3) */
  unsigned int figure[4][4]; /**< Текущая форма тетромино */
} Tetramino_t;

/**
 * @brief Перечисление действий пользователя.
 */
typedef enum {
  Start,     /**< Начать игру */
  Pause,     /**< Поставить игру на паузу */
  Terminate, /**< Завершить игру */
  Left,      /**< Движение влево */
  Right,     /**< Движение вправо */
  Up,        /**< Движение вверх или поворот */
  Down,      /**< Движение вниз */
  Action     /**< Выполнение действия */
} UserAction_t;

/**
 * @brief Перечисление состояний игры.
 */
typedef enum {
  START = 0, /**< Начало игры */
  SPAWN,     /**< Появление новой фигуры */
  MOVING,    /**< Фигура движется */
  SHIFTING,  /**< Сдвиг или поворот фигуры */
  GAMEOVER,  /**< Конец игры */
  EXIT_STATE /**< Выход из игры */
} State_t;

/**
 * @brief Структура, содержащая информацию об игре.
 */
typedef struct {
  int field[ROWS_MAP][COLS_MAP];; /**< Двумерный массив игрового поля */
  int **next; /**< Двумерный массив для отображения следующего тетромино */
  int score;  /**< Текущий счет игрока */
  int high_score; /**< Максимальный достигнутый счет */
  int level;      /**< Текущий уровень */
  int speed;      /**< Скорость падения фигур */
  int pause;      /**< Состояние паузы */
} GameInfo_t;

/**
 * @brief Структура, содержащая параметры игры.
 */
typedef struct GameParams {
  GameInfo_t *stats; /**< Указатель на структуру с информацией об игре */
  State_t *state; /**< Указатель на текущее состояние игры */
  struct timespec *time; /**< Указатель на структуру времени */
  Tetramino_t *tetramino; /**< Указатель на текущее тетромино */
  int sig; /**< Сигнальное значение (для обработки сигналов) */
  int hold; /**< Флаг удержания текущего тетромино */
} Params_t;

#endif