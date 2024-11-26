#include "SnakeController.h"
#include <QDebug>
#include <QApplication>

SnakeController::SnakeController(BrickGame* view, Snake& snakeInstance, QObject* parent)
    : QObject(parent), view(view), snake(snakeInstance) {
  connect(view, &BrickGame::keyPressed, this, &SnakeController::processInput);
  connect(view->getGameTimer(), &QTimer::timeout, this, &SnakeController::updateGame);
}

void SnakeController::processInput(QKeyEvent* event = nullptr) {
    UserAction_t signal = (event) ? inputHandler.GetSignal(event->key()) : Nosig;
    int hold = (signal == Down) ? 1 : 0;

    snake.userInput(signal, hold);
    updateView();

    if (event) {
        qDebug() << "Processing user input: " << event->key();
        qDebug() << "signal -> " << signal;
    }
}

void SnakeController::updateGame() {
    GameInfo_t game_info = snake.updateCurrentState();
    Snake_t snake_info = snake.GetSnakeInfo();
    
    if (game_info.pause == 1) {
        qDebug() << "Game is paused";
        return;
    }
    
    processInput();

    // Проверка состояния игры: GAMEOVER или EXIT_STATE
    if (snake_info.state == GAMEOVER || snake_info.state == EXIT_STATE) {
        // Остановить таймеры при GAMEOVER или EXIT_STATE
        inputTimer->stop();
        view->getGameTimer()->stop();
        
        // Отобразить сообщение о завершении игры
        if (snake_info.state == GAMEOVER) {
            view->showGameOver();
        }
        // Завершаем работу игры (если EXIT_STATE)
        if (Snake_info.state == EXIT_STATE) {
            QApplication::quit(); // Закрытие приложения
        }
        return;
    }

    // Обновление игры, если все в порядке
    updateView();
}

void SumSnake(GameInfo_t *game_info, Tetramino_t tetramino) {
  for (int dx = 0; dx < 4; ++dx) {
    for (int dy = 0; dy < 4; ++dy) {
      if (tetramino.figure[dx][dy] == 1) {
        int x = tetramino.point->x + dx;
        int y = tetramino.point->y + dy;
        game_info->field[x][y] = 1;
      }
    }
  }
}

void SnakeController::updateView() {
  qDebug() << "update view in controller ";
  view->update();
}
