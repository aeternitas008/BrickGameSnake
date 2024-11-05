#ifndef BACKEND_H
#define BACKEND_H

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <array>
#include "../defines.hpp"

/**
 * @file main.h Файл имеет основные функции по реализации игры, а именно
 * чтение и обработку ключей, перемещение тетромино по полю и реализацию базовой
 * механики игры.
 *
 */

/**
 * @brief Структура, представляющая тетромино, включая его позицию, тип и
 * варианты вращения.
 */
struct Tetramino_t{
  Position_t *point; /**< Указатель на позицию тетромино на игровом поле */
  int type; /**< Тип тетромино */
  int next_type; /**< Тип следующего тетромино */
  int variant; /**< Вариант вращения тетромино (0-3) */
  unsigned int figure[4][4]; /**< Текущая форма тетромино */
  std::array<int, 4> last_tetramino;
};

/**
 * @brief Структура, представляющая тетромино, включая его позицию, тип и
 * варианты вращения.
 */
struct Tetris_t{
  Tetramino_t *tetramino;
  struct timespec *time;
  State_t state;
  int hold;
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
  static inline Tetris_t* tetris_;

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
    // Инициализируем статический указатель, если он ещё не инициализирован
    if (!game_info_) {
      game_info_ = new GameInfo_t;
      tetris_ = new Tetris_t;
      tetris_->tetramino = new Tetramino_t;
      tetris_->tetramino->point = new Position_t;
      tetris_->time = new timespec;
      game_info_->pause = 0;
      tetris_->state = START;
      InitBoard(game_info_->field);
      StatsInit();
      tetris_->tetramino->last_tetramino = {-1, -1, -1, -1};
      GetPseudoRandomTypeTetramino();
      tetris_->tetramino->variant = 0;
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

  State_t GetState() {
      return tetris_->state; 
  }
  Tetris_t GetTetrisInfo() {
      return *tetris_; 
  }
  Tetramino_t GetTetramino() {
      return *tetris_->tetramino; 
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
  void MoveUp();
  void MoveRight();
  void MoveLeft();
  void Check();
  void TurnRight();
  int Offset(struct timespec last_time, struct timespec current_time);
  void GamePause() {
    game_info_->pause = 1;
  }
  void GameResume() {
    game_info_->pause = 0;
    tetris_->state = MOVING;
  }
  void GameOver() {
    tetris_->state = EXIT_STATE;
  }
  void ExitState() { tetris_->state = EXIT_STATE; }
  void SetHold(int x) {
    tetris_->hold = x;
  }
};

#endif