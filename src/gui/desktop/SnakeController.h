#ifndef Snake_CONTROLLER_H
#define Snake_CONTROLLER_H

#include "../brick_game/Snake/ModelSnake.h"
#include "InputHandler.h"
#include "BrickGame.h"

class SnakeController : public QObject {
    Q_OBJECT

public:
    explicit SnakeController(BrickGame* view, Snake& snakeInstance, QObject* parent = nullptr);

public slots:
    void processInput(QKeyEvent* event);
    void updateGame();

private:
    BrickGame* view;
    Snake& snake;
    GameInfo_t game_info_board;
    InputHandler inputHandler;

    QTimer* inputTimer;

    void updateView();
};

#endif // Snake_CONTROLLER_H
