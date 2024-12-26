/**
 * @file GameController.h
 * @brief Определяет контроллер для управления игровым процессом.
 *
 * Этот файл описывает класс `GameController`, который обеспечивает
 * связь между пользовательским интерфейсом, обработкой ввода и
 * игровыми данными. Основная задача контроллера — управление игровым
 * циклом и обработка взаимодействия игрока с игрой.
 */

#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "../gui/cli/ConsoleView.h"
#include "Game.h"
#include "InputHandler.h"
#include "snake/ModelSnake.h"

/**
 * @class GameController
 * @brief Контроллер для управления игровым процессом.
 *
 * Обеспечивает взаимодействие между пользовательским интерфейсом,
 * обработкой ввода и игровой логикой. Основной метод `GameLoop` отвечает
 * за запуск и управление игровым циклом.
 */
class GameController {
 public:
  /**
   * @brief Конструктор контроллера игры.
   *
   * @param view Указатель на объект представления (интерфейс пользователя).
   * @param game Ссылка на объект игры.
   */
  GameController(ConsoleView* view, Game& game)
      : view(view), game(game), no_break(true) {}

  /**
   * @brief Основной игровой цикл.
   *
   * Отвечает за обработку событий, обновление состояния игры
   * и отображение игрового интерфейса.
   */
  void GameLoop();

 private:
  ConsoleView* view; /**< Указатель на представление для отображения игры. */
  InputHandler*
      input_handler; /**< Указатель на обработчик пользовательского ввода. */
  Game& game;        /**< Ссылка на объект игры. */
  bool no_break; /**< Флаг для управления игровым циклом. */
};

#endif  // GAME_CONTROLLER_H
