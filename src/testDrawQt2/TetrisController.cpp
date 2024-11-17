#include "TetrisController.h"
#include <QDebug>

TetrisController::TetrisController(BrickGame* view, Tetris& tetrisInstance, QObject* parent)
    : QObject(parent), view(view), tetris(tetrisInstance) {
    connect(view, &BrickGame::keyPressed, this, &TetrisController::processInput);
    connect(view->getGameTimer(), &QTimer::timeout, this, &TetrisController::updateGame);
}

void TetrisController::processInput(QKeyEvent* event) {
    UserAction_t signal = inputHandler.GetSignal(event->key());
    tetris.userInput(signal, 0);
}

void TetrisController::updateGame() {
    tetris.Shifting();
    updateView();

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
    SumTetris(&game_info, current_tetramino);
    view->drawField(game_info);
    // view->updateStats(game_info);
}
