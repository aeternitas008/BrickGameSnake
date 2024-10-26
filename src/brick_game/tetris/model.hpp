#ifndef BACKEND_H
#define BACKEND_H

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <array>

// #include <sys/time.h>
// #include <stdlib.h>

#include "defines.hpp"

/**
 * @file main.h Файл имеет основные функции по реализации игры, а именно
 * чтение и обработку ключей, перемещение тетромино по полю и реализацию базовой
 * механики игры.
 *
 */

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
 * @brief Структура, представляющая координаты на игровом поле.
 */
struct Position{
  int x, y; /**< Координаты */
} ;

/**
 * @brief Структура, представляющая тетромино, включая его позицию, тип и
 * варианты вращения.
 */
struct Tetramino_t{
  Position *point; /**< Указатель на позицию тетромино на игровом поле */
  int type; /**< Тип тетромино */
  int next_type; /**< Тип следующего тетромино */
  int variant; /**< Вариант вращения тетромино (0-3) */
  unsigned int figure[4][4]; /**< Текущая форма тетромино */
  std::array<int, 4> last_tetramino;
};

/**
 * @brief Структура, содержащая информацию об игре.
 */
struct GameInfo_t {
  int field[ROWS_MAP][COLS_MAP]; /**< Двумерный массив игрового поля */
  int next[4][4]; /**< Двумерный массив для отображения следующего тетромино */
  int score;  /**< Текущий счет игрока */
  int high_score; /**< Максимальный достигнутый счет */
  int level;      /**< Текущий уровень */
  int speed;      /**< Скорость падения фигур */
  int pause;      /**< Состояние паузы */
  int realtime[2]; /**< Текущее время */
  int hold; /**< Флаг удержания текущего тетромино */
  struct timespec *time; /**< Указатель на структуру времени */
  Tetramino_t *tetramino; /**< Указатель на текущее тетромино */
  State_t state; /**< Текущее состояние игры */
};

static const unsigned int TETRAMINO_FIGURES[19][4][4] = {
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},

    {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},

    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    // L
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    // J 11
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
    {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    // T 15
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
};

class Tetris {
  private:
  static inline GameInfo_t* game_info_;

  void AddTypeTetramino(int tetramino);
  bool isUnique(int tetramino, const std::array<int, 4>& last_tetraminos);
  void GetPseudoRandomTypeTetramino();
  int HasFullLine(int *line);
  void CheckLines(int *count);
  void NewStatsSaveInit();
  bool CheckFirstLine();

  bool IsNotBlockBelow();

  bool IsBlockRight();
  bool IsBlockLeft();

  void AddTetraminoOnBoard();
  void StatsInit();
  void TetraminoPosInit();
  void InitBoard(int field[ROWS_MAP][COLS_MAP]);
  int CheckNewVariant();
  int CheckTetramino(Tetramino_t tetramino);



  /**
   * @brief Заполняет текущую фигуру тетрамино на основе его типа и варианта.
   *
   * Функция извлекает фигуру тетрамино на основе его типа и варианта,
   * и копирует её в поле figure структуры Tetramino_t.
   *
   * @param tetramino Указатель на структуру Tetramino_t, в которую будет
   * скопирована фигура.
   */
  void GetFigure(Tetramino_t *tetramino);

  public:

  Tetris() {
    if (!game_info_) {
      game_info_ = new GameInfo_t;
      game_info_->time = new timespec;      // Инициализация указателя
      clock_gettime(CLOCK_REALTIME, game_info_->time);
      game_info_->tetramino = new Tetramino_t;
      game_info_->tetramino->point = new Position;
      game_info_->pause = 0;
    // Инициализируем статический указатель, если он ещё не инициализирован
      game_info_->state = START;
      InitBoard(game_info_->field);
      StatsInit();
      game_info_->tetramino->last_tetramino = {-1, -1, -1, -1};
      GetPseudoRandomTypeTetramino();
      game_info_->tetramino->variant = 0;
    }
  }

  // ~Tetris() {
  //   if (game_info_) {
  //     if (game_info_->tetramino->point) {
  //     delete game_info_->tetramino->point;
  //     game_info_->tetramino->point = nullptr;  // Установите указатель в null после удаления
  //   }
  //     if (game_info_->tetramino) {
  //     delete game_info_->tetramino;
  //     game_info_->tetramino = nullptr;  // Установите указатель в null после удаления
  //   }
  //     if (game_info_->time) {
  //     delete game_info_->time;
  //     game_info_->time = nullptr;  // Установите указатель в null после удаления
  //   }
  //   delete game_info_;  // Освобождаем память для game_info
  //   }
  // }

  // Пример функции, изменяющей состояние игры
    // void UpdateState(State_t new_state) {
    //     game_info_.state = new_state; // Меняем состояние через статическую переменную
    // }

  State_t GetState() {
      return game_info_->state; 
  }
  /**
   * @brief Возвращает указатель на статический экземпляр структуры GameInfo_t.
   *
   * Функция инициализирует и возвращает указатель на статический объект GameInfo_t.
   * Используется для работы с параметрами игры в течение всего времени её
   * выполнения.
   *
   * @return Указатель на структуру GameInfo_t.
   */
  static GameInfo_t updateCurrentState() {
    return *game_info_;
  }

  void StartGame();
  void Spawn();

  /**
   * @brief Обновляет положение тетрамино на поле через определённый интервал
   * времени.
   *
   * Функция вычисляет прошедшее время и сдвигает тетрамино вниз, если прошло
   * достаточно времени с момента последнего обновления. Уровень сложности игры
   * также влияет на скорость падения.
   *
   * @param game_info_ Указатель на структуру GameInfo_t с параметрами игры.
   */
  void Shifting();

  void MoveUp();
  /**
   * @brief Сдвигает тетрамино вниз, если под ним нет блокирующих элементов.
   *
   * Функция проверяет, можно ли сдвинуть тетрамино вниз. Если можно, тетрамино
   * сдвигается. Если оно достигает препятствия или дна поля, тетрамино
   * добавляется на игровую доску.
   *
   * @param game_info_ Указатель на структуру GameInfo_t с параметрами игры.
   */
  void MoveDown();

  void MoveRight();
  void MoveLeft();
  void GameOver();
  void ExitState();
  void Check();
  void TurnRight();
  void GamePause();
  void GameResume();

  void GetRealTime();
  int Offset(struct timespec last_time, struct timespec current_time);

  void SetHold(int x) {
    game_info_->hold = x;
  }
};


#endif