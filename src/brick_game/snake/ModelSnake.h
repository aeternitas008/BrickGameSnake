#ifndef SNAKE_H
#define SNAKE_H

#include <locale.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <deque>
#include <cstdio>

#include "../defines.hpp"

/**
 * @file main.h Файл имеет основные функции по реализации игры, а именно
 * чтение и обработку ключей, перемещение тетромино по полю и реализацию базовой
 * механики игры.
 *
 */

/**
 * @brief Перечисление направлений движения змейки.
 */
enum Direction_t {
  UP_DIRECTION = 0, /**< Начало игры */
  RIGHT_DIRECTION,    /**< Фигура движется */
  DOWN_DIRECTION,  /**< Сдвиг или поворот фигуры */
  LEFT_DIRECTION,     /**< Появление новой фигуры */
};

struct Snake_t {
  std::deque<Position_t> points; /**< Координаты точек змейки */
  Direction_t direction; /**< Направление движения змейки (0-3) */
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

  void userInput(UserAction_t signal, bool hold);

  State_t GetState() {
      return snake_info_->state; 
  }
  
  static GameInfo_t updateCurrentState() {
    return *game_info_;
  }

  static SnakeInfo_t GetSnakeInfo() {
    return *snake_info_;
  }

  Snake() {
    if (!game_info_) {
      game_info_ = new GameInfo_t;
      snake_info_ = new SnakeInfo_t;
      snake_info_->time = new timespec;
      snake_info_->snake = new Snake_t;
      snake_info_->apple = new Position_t;
      snake_info_->snake->direction = LEFT_DIRECTION;
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

  void StartGame();
  void Spawn();
  void Shifting();
  void MoveUp();
  void MoveForward();
  void MoveRight();
  void MoveDown();
  void MoveLeft();
  void Check();
  int Offset(struct timespec last_time, struct timespec current_time);
  
  void GameOver() {
    snake_info_->state = EXIT_STATE;
  }
  void GamePause() {
    game_info_->pause = (game_info_->pause == 0);
  }
  void ExitState() { snake_info_->state = EXIT_STATE; }
};

#endif //SNAKE_H