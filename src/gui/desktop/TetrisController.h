#ifndef TETRIS_CONTROLLER_H
#define TETRIS_CONTROLLER_H

#include "../../brick_game/tetris/ModelTetris.h"
#include "InputHandler.h"
#include "BrickGame.h"

class TetrisController : public QObject {
    Q_OBJECT

public:
    explicit TetrisController(BrickGame* view, Tetris& tetrisInstance, QObject* parent = nullptr);

public slots:
    void processInput(QKeyEvent* event);
    void updateGame();

private:
    BrickGame* view;
    Tetris& tetris;
    GameInfo_t game_info_board;
    InputHandler inputHandler;

    QTimer* inputTimer;

    void updateView();
};

#endif // TETRIS_CONTROLLER_H
