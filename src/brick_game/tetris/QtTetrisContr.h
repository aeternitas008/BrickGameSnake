#ifndef TETRIS_CONTROLLER_H
#define TETRIS_CONTROLLER_H

#include "ModelTetris.h"
#include "../InputHandler.h"
#include "../../gui/desktop/BrickGame.h"

class TetrisController : public QObject {
    Q_OBJECT

public:
    explicit TetrisController(BrickGame* view, Tetris& tetrisInstance, QObject* parent = nullptr);
    void handleKeyPress(QKeyEvent* event);

    ~TetrisController() override;

public slots:
    void processInput(QKeyEvent* event);
    void updateGame();

private:
    BrickGame* view;
    Tetris& tetris;
    InputHandler* inputHandler;

    QTimer* inputTimer;
    QTimer* gameTimer;  // Таймер контроллера
    void updateView();
    void pauseGame();

    bool isPaused = false;
    bool isStartGame = false;
    bool isGameStarted = false;

};

#endif // TETRIS_CONTROLLER_H
