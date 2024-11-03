#ifndef SNAKE_H
#define SNAKE_H

#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <deque>

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
 * @brief Перечисление направлений движения змейки.
 */
enum Direction_t {
  UP_DIRECTION = 0, /**< Начало игры */
  RIGHT_DIRECTION,    /**< Фигура движется */
  DOWN_DIRECTION,  /**< Сдвиг или поворот фигуры */
  LEFT_DIRECTION,     /**< Появление новой фигуры */
};

/**
 * @brief Структура, представляющая координаты на игровом поле.
 */
struct Position_t{
  int x, y; /**< Координаты */
};

/**
 * @brief Структура, представляющая тетромино, включая его позицию, тип и
 * варианты вращения.
 */
struct Snake_t {
  std::deque<Position_t> points; /**< Координаты точек змейки */
  Direction_t direction; /**< Направление движения змейки (0-3) */
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
};

struct SnakeInfo_t {
  struct timespec *time; /**< Указатель на структуру времени */
  Snake_t *snake; /**< Указатель на змейку */
  Position_t *apple; /**< Указатель на позицию яблока */
  State_t state; /**< Текущее состояние игры */
};

class Snake {
  private:
  static inline GameInfo_t* game_info_;
  static inline SnakeInfo_t* snake_info_;

  void NewStatsSaveInit();
  void StatsInit();
  void SnakePosInit();
  void GetRandomCoordiantes();
  void InitBoard();
  int IsCollision(Position_t point);
  int IsEating(Position_t new_point);

  int IsBodySnake(Position_t point);

  public:

  Snake() {
    if (!game_info_) {
      game_info_ = new GameInfo_t;
      snake_info_ = new SnakeInfo_t;
      snake_info_->time = new timespec;      // Инициализация указателя
      // clock_gettime(CLOCK_REALTIME, game_info_->time);
      snake_info_->snake = new Snake_t;
      snake_info_->apple = new Position_t;
      snake_info_->snake->direction = LEFT_DIRECTION;
      // game_info_->snake->points = new std::deque<Position_t>;
      game_info_->pause = 0;
    // Инициализируем статический указатель, если он ещё не инициализирован
      snake_info_->state = START;
      InitBoard();
      SnakePosInit();
      GetRandomCoordiantes();
      StatsInit();
    }
  }

  // ~Snake() {
  //   if (game_info_) {
  //     if (game_info_->snake->point) {
  //     delete game_info_->snake->point;
  //     game_info_->snake->point = nullptr;  // Установите указатель в null после удаления
  //   }
  //     if (game_info_->snake) {
  //     delete game_info_->snake;
  //     game_info_->snake = nullptr;  // Установите указатель в null после удаления
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
      return snake_info_->state; 
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

  static SnakeInfo_t GetSnakeInfo() {
    return *snake_info_;
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
  void MoveForward();

  void MoveRight();
  void MoveDown();
  void MoveLeft();
  void GameOver();
  void ExitState();
  void Check();
  void GamePause();
  void GameResume();
  int Offset(struct timespec last_time, struct timespec current_time);

};


#endif //SNAKE_H