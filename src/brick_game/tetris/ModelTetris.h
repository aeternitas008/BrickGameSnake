/**
 * @file ModelTetris.h
 * @brief Заголовочный файл для игры "Тетрис".
 *
 * Содержит определения структур, констант, и класса Tetris, который наследует
 * базовый класс Game. Реализует функциональность игры "Тетрис", включая
 * генерацию фигур, управление, обработку линий, сохранение рекордов и
 * обновление игрового состояния.
 */

#ifndef TETRIS_H
#define TETRIS_H

#include <locale.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <array>
#include <cstdio>

#include "../Game.h"

/**
 * @brief Константный массив, содержащий формы всех возможных тетромино.
 *
 * Массив определяет 19 возможных вариантов фигур (включая повороты)
 * на игровом поле размером 4x4. Каждое число указывает наличие или отсутствие
 * блока.
 */
static const unsigned int TETRAMINO_FIGURES[19][4][4] = {
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},  // O

    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},

    {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},  // S
    {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},

    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},  // Z
    {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},

    // L
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},

    // J
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
    {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},

    // T
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
};

/**
 * @struct Tetramino_t
 * @brief Структура, описывающая тетромино в игре.
 */
struct Tetramino_t {
  Position_t *point; /**< Указатель на позицию тетромино на игровом поле. */
  int type;          /**< Тип текущего тетромино. */
  int next_type;     /**< Тип следующего тетромино. */
  int variant = 0; /**< Вариант вращения тетромино (0-3). */
  unsigned int figure[4][4]; /**< Текущая форма тетромино. */
  std::array<int, 4> last_tetramino = {
      -1, -1, -1, -1}; /**< Последние типы фигур для случайной генерации. */
};

/**
 * @struct Tetris_t
 * @brief Структура, содержащая информацию о состоянии игры "Тетрис".
 */
struct Tetris_t {
  Tetramino_t *tetramino; /**< Указатель на текущую фигуру. */
  struct timespec *time; /**< Указатель на структуру времени. */
  int hold; /**< Указывает, активировано ли удержание фигуры. */
};

/**
 * @class Tetris
 * @brief Класс, реализующий игру "Тетрис".
 */
class Tetris : public Game {
 private:
  static inline Tetris_t
      *tetris_; /**< Указатель на информацию об игре "Тетрис". */

 public:
  /**
   * @brief Конструктор класса Tetris.
   *
   * Инициализирует структуры для игры и запускает начальную настройку.
   */
  Tetris();

  /**
   * @brief Деструктор класса Snake.
   *
   * Очищает память выделенную для структуры.
   */
  void DestructorTetris() {
    if (tetris_) {
        delete tetris_->tetramino;
        delete tetris_->tetramino->point;
        delete tetris_->time;
        delete tetris_;
        tetris_ = nullptr;
      }
    }

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
    SumTetris(&game, *tetris_->tetramino);
    return game;
  }

  /**
   * @brief Запускает игру.
   */
  void StartGame();

  /**
   * @brief Добавляет тип тетромино в список.
   *
   * @param tetramino Тип тетромино для добавления.
   */
  void AddTypeTetramino(int tetramino);

  /**
   * @brief Проверяет уникальность типа тетромино.
   *
   * @param tetramino Тип тетромино для проверки.
   * @param last_tetraminos Последние четыре типа тетромино.
   * @return true, если тип уникален, иначе false.
   */
  bool isUnique(int tetramino, const std::array<int, 4> &last_tetraminos);

  /**
   * @brief Генерирует случайный тип тетромино.
   */
  void GetPseudoRandomTypeTetramino();

  /**
   * @brief Проверяет наличие полной линии на игровом поле.
   *
   * @param line Указатель на индекс линии.
   * @return int Возвращает 1, если линия заполнена, иначе 0.
   */
  int HasFullLine(int *line);

  /**
   * @brief Проверяет и очищает заполненные линии.
   *
   * @param count Указатель на количество очищенных линий.
   */
  void CheckLines(int *count);

  /**
   * @brief Проверяет первую линию на наличие блоков.
   *
   * @return true, если линия заполнена, иначе false.
   */
  bool CheckFirstLine();

  /**
   * @brief Проверяет, есть ли блоки ниже текущего тетромино.
   *
   * @return true, если блоков нет, иначе false.
   */
  bool IsNotBlockBelow();

  /**
   * @brief Проверяет, есть ли блок справа от текущего тетромино.
   *
   * @return true, если блоков нет, иначе false.
   */
  bool IsBlockRight();

  /**
   * @brief Проверяет, есть ли блок слева от текущего тетромино.
   *
   * @return true, если блоков нет, иначе false.
   */
  bool IsBlockLeft();

  /**
   * @brief Добавляет текущее тетромино на игровое поле.
   */
  void AddTetraminoOnBoard();

  /**
   * @brief Инициализирует сохранение рекордов (для тестирования).
   */
  void NewStatsSaveInit() { StatsSave(SCORE_FILE_TTR); };

  /**
   * @brief Инициализирует статистику игры.
   */
  void StatsInit() override { ReadHighScore(SCORE_FILE_TTR); };

  /**
   * @brief Инициализирует начальную позицию тетромино.
   */
  void TetraminoPosInit();

  /**
   * @brief Проверяет новый вариант вращения фигуры.
   *
   * @return int Возвращает 1, если вариант допустим, иначе 0.
   */
  int CheckNewVariant();

  /**
   * @brief Проверяет корректность расположения тетромино.
   *
   * @param tetramino Проверяемое тетромино.
   * @return int Возвращает 1, если расположение допустимо, иначе 0.
   */
  int CheckTetramino(Tetramino_t tetramino);

  /**
   * @brief Суммирует текущее состояние игры и тетромино.
   *
   * @param game_info Указатель на информацию о текущем состоянии игры.
   * @param tetramino Информация о текущем тетромино.
   */
  void SumTetris(GameInfo_t *game_info, Tetramino_t tetramino);

  /**
   * @brief Получает текущую фигуру.
   *
   * @param tetramino Указатель на структуру тетромино.
   */
  void GetFigure(Tetramino_t *tetramino);

  /**
   * @brief Создаёт новую фигуру.
   */
  void Spawn();

  /**
   * @brief Выполняет сдвиг фигуры.
   */
  void Shifting();

  /**
   * @brief Перемещает фигуру вниз.
   */
  void MoveDown();

  /**
   * @brief Перемещает фигуру вверх.
   */
  void MoveUp();

  /**
   * @brief Перемещает фигуру вправо.
   */
  void MoveRight();

  /**
   * @brief Перемещает фигуру влево.
   */
  void MoveLeft();

  /**
   * @brief Проверяет текущее состояние игры (линии, столкновения и т.д.).
   */
  void Check();

  /**
   * @brief Выполняет вращение фигуры вправо.
   */
  void TurnRight();

  // /**
  //  * @brief Ставит игру на паузу или снимает с паузы.
  //  */
  // void GamePause() {
  //     game_info_->pause = !game_info_->pause;
  // }

  // /**
  //  * @brief Завершает игру.
  //  */
  // void GameOver() {
  //     game_info_->state = EXIT_STATE;
  // }

  // /**
  //  * @brief Переводит игру в состояние выхода.
  //  */
  // void ExitState() {
  //     game_info_->state = EXIT_STATE;
  // }

  /**
   * @brief Возвращает информацию о текущей игре.
   *
   * @return Tetris_t Информация об игре.
   */
  Tetris_t getTetrisInfo() { return *tetris_; }

  /**
   * @brief Устанавливает текущее состояние игры.
   *
   * @param game_info Новая информация об игре.
   */
  void setGameInfo(GameInfo_t game_info) { *game_info_ = game_info; }
};

#endif  // TETRIS_H
