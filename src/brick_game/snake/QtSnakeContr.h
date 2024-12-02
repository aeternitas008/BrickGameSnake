#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

#include "ModelSnake.h"
#include "../InputHandler.h"
#include "../../gui/desktop/BrickGame.h"

class SnakeController : public QObject {
    Q_OBJECT

public:
    explicit SnakeController(BrickGame* view, Snake& snakeInstance, QObject* parent = nullptr);
    void handleKeyPress(QKeyEvent* event);

    ~SnakeController() override;

public slots:
    void processInput(QKeyEvent* event);
    void updateGame();

private:
    BrickGame* view;
    Snake& snake;
    InputHandler* inputHandler;

    QTimer* inputTimer;
    QTimer* gameTimer;  // Таймер контроллера
    void updateView();
    void pauseGame();

    bool isPaused = false;
    bool isStartGame = false;
    bool isGameStarted = false;
};

#endif // SNAKE_CONTROLLER_H