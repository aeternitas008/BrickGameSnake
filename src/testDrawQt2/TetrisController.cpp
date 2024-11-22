#include "TetrisController.h"
#include <QDebug>

TetrisController::TetrisController(BrickGame* view, Tetris& tetrisInstance, QObject* parent)
    : QObject(parent), view(view), tetris(tetrisInstance) {
    connect(view, &BrickGame::keyPressed, this, &TetrisController::processInput);
    connect(view->getGameTimer(), &QTimer::timeout, this, &TetrisController::updateGame);
}

void TetrisController::processInput(QKeyEvent* event) {
    int hold = 0;
    qDebug() << "key -> " << event->key();
    UserAction_t signal = inputHandler.GetSignal(event->key());
    qDebug() << "signal -> " << signal;
    if (signal == Down) hold = 1;
    tetris.userInput(signal, hold);
    view->update();
}

void TetrisController::updateGame() {
    // tetris.Shifting();
    QKeyEvent* fakeEvent = new QKeyEvent(QEvent::KeyPress, 10, Qt::NoModifier);
    processInput(fakeEvent);
    updateView();
    qDebug() << "update game in controller ";
    // Проверка состояния игры
    Tetris_t tetrisInfo = tetris.GetTetrisInfo();
    if (tetrisInfo.state == GAMEOVER) {
        view->getGameTimer()->stop();
        view->showGameOver();
    } else if (tetrisInfo.state == EXIT_STATE) {
        view->getGameTimer()->stop();
    }
}

void SumTetris(GameInfo_t *game_info, Tetramino_t tetramino) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetramino.figure[x][y] == 1) {
        game_info->field[tetramino.point->x + x]
                        [tetramino.point->y + y] = 1;
      }
    }
  }
}

void TetrisController::updateView() {
    GameInfo_t game_info = tetris.updateCurrentState();
    Tetramino_t current_tetramino = tetris.GetTetramino();
    game_info_board = game_info;
    SumTetris(&game_info_board, current_tetramino);
    
    qDebug() << "controller ";
    // view->update();
}
