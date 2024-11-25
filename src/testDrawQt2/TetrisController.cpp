#include "TetrisController.h"
#include <QDebug>
#include <QApplication>

TetrisController::TetrisController(BrickGame* view, Tetris& tetrisInstance, QObject* parent)
    : QObject(parent), view(view), tetris(tetrisInstance) {
  connect(view, &BrickGame::keyPressed, this, &TetrisController::processInput);
  connect(view->getGameTimer(), &QTimer::timeout, this, &TetrisController::updateGame);
}

void TetrisController::processInput(QKeyEvent* event = nullptr) {
    UserAction_t signal = (event) ? inputHandler.GetSignal(event->key()) : Nosig;
    int hold = (signal == Down) ? 1 : 0;

    tetris.userInput(signal, hold);
    updateView();

    if (event) {
        qDebug() << "Processing user input: " << event->key();
        qDebug() << "signal -> " << signal;
    }
}

void TetrisController::updateGame() {
    GameInfo_t game_info = tetris.updateCurrentState();
    Tetris_t tetris_info = tetris.GetTetrisInfo();
    
    if (game_info.pause == 1) {
        qDebug() << "Game is paused";
        return;
    }
    
    processInput();

    // Проверка состояния игры: GAMEOVER или EXIT_STATE
    if (tetris_info.state == GAMEOVER || tetris_info.state == EXIT_STATE) {
        // Остановить таймеры при GAMEOVER или EXIT_STATE
        inputTimer->stop();
        view->getGameTimer()->stop();
        
        // Отобразить сообщение о завершении игры
        if (tetris_info.state == GAMEOVER) {
            view->showGameOver();
        }
        // Завершаем работу игры (если EXIT_STATE)
        if (tetris_info.state == EXIT_STATE) {
            QApplication::quit(); // Закрытие приложения
        }
        return;
    }

    // Обновление игры, если все в порядке
    updateView();
}

void SumTetris(GameInfo_t *game_info, Tetramino_t tetramino) {
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

void TetrisController::updateView() {
  qDebug() << "update view in controller ";
  view->update();
}
