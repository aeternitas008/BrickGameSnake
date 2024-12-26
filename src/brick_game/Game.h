#ifndef GAME_H
#define GAME_H

#include <time.h>

#include <cstdio>
#include <cstdlib>

#include "defines.h"

/**
 * @brief Базовый класс для всех игровых логик.
 *
 * Этот класс предоставляет основные методы и структуры данных для управления
 * состоянием игры, обработки ввода пользователя, работы с файлами статистики и
 * времени.
 */
class Game {
 protected:
  /**
   * @brief Указатель на структуру с информацией о текущем состоянии игры.
   */
  static inline GameInfo_t* game_info_;

  /**
   * @brief Инициализация статистики игры.
   *
   * Этот метод должен быть реализован в производных классах для настройки
   * начальной статистики.
   */
  virtual void StatsInit() = 0;

  /**
   * @brief Вычисляет смещение времени в миллисекундах между двумя временными
   * точками.
   *
   * @param last_time Время начала.
   * @param current_time Текущее время.
   * @return Смещение времени в миллисекундах.
   */
  int Offset(struct timespec last_time, struct timespec current_time) {
    return (current_time.tv_sec - last_time.tv_sec) * 1000 +
           (current_time.tv_nsec - last_time.tv_nsec) / 1000000;
  }

 public:
  /**
   * @brief Конструктор. Инициализирует информацию о состоянии игры.
   */
  Game() {
    if (!game_info_) {
      game_info_ = new GameInfo_t;
    }
  }

  /**
   * @brief Обрабатывает ввод пользователя.
   *
   * @param signal Действие пользователя.
   * @param hold Флаг удержания действия.
   */
  virtual void userInput(UserAction_t signal, bool hold) = 0;

  /**
   * @brief Обновляет и возвращает текущее состояние игры.
   *
   * @return Текущее состояние игры.
   */
  virtual GameInfo_t updateCurrentState() { return *game_info_; }

  /**
   * @brief Считывает рекорд из файла.
   *
   * Если файл не существует, он создается с начальным значением рекорда 0.
   *
   * @param file_name Имя файла для сохранения рекорда.
   */
  void ReadHighScore(const char* file_name) {
    int N = 256;
    char buffer[N];
    FILE* fp = fopen(file_name, "r");
    if (fp != NULL) {
      fgets(buffer, N, fp);
      game_info_->high_score = atoi(buffer);
      fclose(fp);
    } else {
      FILE* file = fopen(file_name, "w");
      fprintf(file, "0");
      fclose(file);
      game_info_->high_score = 0;
    }
  }

  /**
   * @brief Сохраняет текущий рекорд в файл.
   *
   * Рекорд обновляется только в случае, если текущий счёт выше сохранённого
   * рекорда.
   *
   * @param file_name Имя файла для сохранения рекорда.
   */
  void StatsSave(const char* file_name) {
    if (game_info_->score > game_info_->high_score) {
      game_info_->high_score = game_info_->score;
      FILE* file = fopen(file_name, "w");
      fprintf(file, "%d", game_info_->high_score);
      fclose(file);
    }
  }

  /**
   * @brief Ставит игру на паузу или снимает с паузы.
   */
  void GamePause() { game_info_->pause = !game_info_->pause; }

  /**
   * @brief Завершает игру.
   */
  void GameOver() { game_info_->state = EXIT_STATE; }

  /**
   * @brief Переводит игру в состояние выхода.
   */
  void ExitState() { game_info_->state = EXIT_STATE; }
};

#endif  // GAME_H
