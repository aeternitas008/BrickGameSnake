#ifndef SNAKE_H
#define SNAKE_H

#include <locale.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <deque>
#include <cstdio>

#include "../Game.h"

enum Direction_t {
  UP_DIRECTION = 0, /**< Начало игры */
  RIGHT_DIRECTION,    /**< Фигура движется */
  DOWN_DIRECTION,  /**< Сдвиг или поворот фигуры */
  LEFT_DIRECTION,     /**< Появление новой фигуры */
};

struct Snake_t {
  std::deque<Position_t> points = {
    {SNAKE_START_X, SNAKE_START_Y},
    {SNAKE_START_X, SNAKE_START_Y + 1},
    {SNAKE_START_X, SNAKE_START_Y + 2},
    {SNAKE_START_X, SNAKE_START_Y + 3}
  }; /**< Координаты точек змейки */
  Direction_t direction = LEFT_DIRECTION; /**< Направление движения змейки (0-3) */
};

struct SnakeInfo_t {
  struct timespec *time; /**< Указатель на структуру времени */
  Snake_t *snake; /**< Указатель на змейку */
  Position_t *apple; /**< Указатель на позицию яблока */
};

class Snake : public Game {
  private:
  static inline GameInfo_t* game_info_;
  static inline SnakeInfo_t* snake_info_;

  // @todo
  void NewStatsSaveInit() {
    StatsSave(SCORE_FILE_TTR);
  };

  // @todo
  void StatsInit() override {
    ReadHighScore(SCORE_FILE_TTR);
  };

  void GetRandomCoordiantes();
  int IsCollision(Position_t point);
  int IsEating(Position_t new_point);
  int IsBodySnake(Position_t point);
  void SumSnake(GameInfo_t *game_info, SnakeInfo_t snake_info);

  void Spawn();
  void Shifting();
  void MoveUp();
  void MoveForward();
  void MoveRight();
  void MoveDown();
  void MoveLeft();
  void Check();

  void GamePause() {
      game_info_->pause = !game_info_->pause;
  }

  void GameOver() {
      game_info_->state = EXIT_STATE;
  }

  void ExitState() {
      game_info_->state = EXIT_STATE;

  }

  public:

  void StartGame() override;
  void userInput(UserAction_t signal, bool hold) override;
  
  GameInfo_t updateCurrentState() override {
    GameInfo_t game = *game_info_;
    SumSnake(&game, *snake_info_);
    return game;
  }

  Snake() : Game()  {
    if (!game_info_) {
      game_info_ = new GameInfo_t;
      snake_info_ = new SnakeInfo_t;
      snake_info_->time = new timespec;
      snake_info_->snake = new Snake_t;
      snake_info_->apple = new Position_t;
      GetRandomCoordiantes();
      StatsInit();
    }
  }
};

#endif //SNAKE_H