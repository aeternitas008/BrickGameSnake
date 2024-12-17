#ifndef BACKEND_H
#define BACKEND_H

#include <locale.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <array>
#include <cstdio>
#include "../Game.h"

struct Tetramino_t{
  Position_t *point; /**< Указатель на позицию тетромино на игровом поле */
  int type; /**< Тип тетромино */
  int next_type; /**< Тип следующего тетромино */
  int variant = 0; /**< Вариант вращения тетромино (0-3) */
  unsigned int figure[4][4]; /**< Текущая форма тетромино */
  std::array<int, 4> last_tetramino = {-1,-1,-1,-1};
};
struct Tetris_t{
  Tetramino_t *tetramino;
  struct timespec *time;
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

class Tetris : public Game {
  private:
  static inline Tetris_t* tetris_;

  void AddTypeTetramino(int tetramino);
  bool isUnique(int tetramino, const std::array<int, 4>& last_tetraminos);
  void GetPseudoRandomTypeTetramino();
  int HasFullLine(int *line);
  void CheckLines(int *count);
  bool CheckFirstLine();

  bool IsNotBlockBelow();
  bool IsBlockRight();
  bool IsBlockLeft();

  void AddTetraminoOnBoard();

  void NewStatsSaveInit() {
    StatsSave(SCORE_FILE_TTR);
  };
  
  void StatsInit() override {
    ReadHighScore(SCORE_FILE_TTR);
  };

  void TetraminoPosInit();
  int CheckNewVariant();
  int CheckTetramino(Tetramino_t tetramino);

  void SumTetris(GameInfo_t *game_info, Tetramino_t tetramino);
  void GetFigure(Tetramino_t *tetramino);

  void Spawn();
  void Shifting();
  void MoveDown();
  void MoveUp();
  void MoveRight();
  void MoveLeft();
  void Check();
  void TurnRight();

  void GamePause() {
      game_info_->pause = !game_info_->pause;
  }

  void GameOver() {
      game_info_->state = EXIT_STATE;
  }

  void ExitState() {
      game_info_->state = EXIT_STATE;

  }

  friend class SnakeTest;

  public:
  Tetris() : Game() {
    // Инициализируем статический указатель, если он ещё не инициализирован
    if (!tetris_) {
      tetris_ = new Tetris_t;
      tetris_->tetramino = new Tetramino_t;
      tetris_->tetramino->point = new Position_t;
      tetris_->time = new timespec;
      StatsInit();
      GetPseudoRandomTypeTetramino();
    }
  }

  void StartGame() override;
  void userInput(UserAction_t signal, bool hold) override;

  GameInfo_t updateCurrentState() override {
    GameInfo_t game = *game_info_;
    SumTetris(&game, *tetris_->tetramino);
    return game;
  }
};

#endif