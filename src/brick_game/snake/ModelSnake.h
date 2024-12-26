/**
 * @file ModelSnake.h
 * @brief Заголовочный файл для игры "Змейка".
 *
 * Содержит определения структур, перечислений и класса Snake, который наследует
 * базовый класс Game. Реализует функциональность игры "Змейка", включая
 * управление змейкой, обработку столкновений, поедание яблок, сохранение
 * рекордов и обновление игрового состояния.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <locale.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <cstdio>
#include <deque>

#include "../Game.h"

/**
 * @enum Direction_t
 * @brief Перечисление направлений движения змейки.
 */
enum Direction_t {
  UP_DIRECTION = 0, /**< Движение вверх. */
  RIGHT_DIRECTION,  /**< Движение вправо. */
  DOWN_DIRECTION,   /**< Движение вниз. */
  LEFT_DIRECTION,   /**< Движение влево. */
};

/**
 * @struct Snake_t
 * @brief Структура, описывающая змейку.
 */
struct Snake_t {
  std::deque<Position_t> points = {
      {SNAKE_START_X, SNAKE_START_Y},
      {SNAKE_START_X, SNAKE_START_Y + 1},
      {SNAKE_START_X, SNAKE_START_Y + 2},
      {SNAKE_START_X, SNAKE_START_Y + 3}}; /**< Координаты точек змейки. */
  Direction_t direction = LEFT_DIRECTION; /**< Направление движения змейки. */
};

/**
 * @struct SnakeInfo_t
 * @brief Структура, содержащая информацию о состоянии игры.
 */
struct SnakeInfo_t {
  struct timespec *time; /**< Указатель на структуру времени. */
  Snake_t *snake;    /**< Указатель на змейку. */
  Position_t *apple; /**< Указатель на позицию яблока. */
};

/**
 * @class Snake
 * @brief Класс, реализующий игру "Змейка".
 */
class Snake : public Game {
 private:
  static inline SnakeInfo_t
      *snake_info_; /**< Указатель на текущую информацию о змейке. */

 public:
  /**
   * @brief Запускает игру.
   */
  void StartGame();

  /**
   * @brief Обрабатывает пользовательский ввод.
   *
   * @param signal Тип действия пользователя.
   * @param hold Указывает, удерживается ли кнопка.
   */
  void userInput(UserAction_t signal, bool hold) override;

  /**
   * @brief Обновляет текущее состояние игры.
   *
   * @return GameInfo_t Текущее состояние игры.
   */
  GameInfo_t updateCurrentState() override {
    GameInfo_t game = *game_info_;
    SumSnake(&game, *snake_info_);
    return game;
  }

  /**
   * @brief Конструктор класса Snake.
   *
   * Инициализирует структуры для игры и запускает начальную настройку.
   */
  Snake();

  /**
   * @brief Инициализирует сохранение рекордов (для тестирования).
   */
  void NewStatsSaveInit() { StatsSave(SCORE_FILE_SNK); };

  /**
   * @brief Инициализирует статистику игры.
   */
  void StatsInit() override { ReadHighScore(SCORE_FILE_SNK); };

  /**
   * @brief Генерирует случайные координаты для яблока.
   */
  void GetRandomCoordiantes();

  /**
   * @brief Проверяет столкновение змейки с заданной точкой.
   *
   * @param point Точка для проверки.
   * @return int 1, если есть столкновение, иначе 0.
   */
  int IsCollision(Position_t point);

  /**
   * @brief Проверяет, съела ли змейка яблоко.
   *
   * @param new_point Новая точка головы змейки.
   * @return int 1, если яблоко съедено, иначе 0.
   */
  int IsEating(Position_t new_point);

  /**
   * @brief Проверяет, находится ли точка внутри тела змейки.
   *
   * @param point Точка для проверки.
   * @return int 1, если точка внутри тела змейки, иначе 0.
   */
  int IsBodySnake(Position_t point);

  /**
   * @brief Суммирует состояние змейки с текущим игровым состоянием.
   *
   * @param game_info Указатель на информацию о текущем состоянии игры.
   * @param snake_info Информация о змейке.
   */
  void SumSnake(GameInfo_t *game_info, SnakeInfo_t snake_info);

  /**
   * @brief Создаёт новую змейку.
   */
  void Spawn();

  /**
   * @brief Выполняет сдвиг змейки.
   */
  void Shifting();

  /**
   * @brief Перемещает змейку вверх.
   */
  void MoveUp();

  /**
   * @brief Перемещает змейку вперёд в зависимости от направления.
   */
  void MoveForward();

  /**
   * @brief Перемещает змейку вправо.
   */
  void MoveRight();

  /**
   * @brief Перемещает змейку вниз.
   */
  void MoveDown();

  /**
   * @brief Перемещает змейку влево.
   */
  void MoveLeft();

  /**
   * @brief Проверяет текущее состояние змейки (столкновения, поедание и т.д.).
   */
  void Check();

  /**
   * @brief Возвращает информацию о текущей змейке.
   *
   * @return SnakeInfo_t Информация о змейке.
   */
  SnakeInfo_t getSnakeInfo() { return *snake_info_; }

  /**
   * @brief Устанавливает текущее состояние игры.
   *
   * @param state Новое состояние игры.
   */
  void setState(State_t state) { game_info_->state = state; }
};

#endif  // SNAKE_H
