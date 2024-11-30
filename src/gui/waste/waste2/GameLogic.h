#include <QPainter>
#include "../brick_game/tetris/ModelTetris.h"
#include "InputHandler.h"
#include "BrickGame.h"

class GameLogic {
public:
    virtual ~GameLogic() = default;

    virtual void updateState() = 0; // Обновление состояния игры
    virtual void handleInput(UserAction_t action) = 0; // Обработка ввода
    virtual void render(QPainter& painter) = 0; // Отрисовка

    virtual bool isGameOver() const = 0;
    virtual bool isPaused() const = 0;
};

class TetrisLogic : public GameLogic {
    Tetris tetris;
public:
    void updateState() override {
        tetris.updateCurrentState();
    }

    void handleInput(UserAction_t action) override {
        tetris.userInput(action);
    }

    void render(QPainter& painter) override {
        // Логика отрисовки для Тетриса
    }

    bool isGameOver() const override {
        return tetris.GetTetrisInfo().state == GAMEOVER;
    }

    bool isPaused() const override {
        return tetris.updateCurrentState().pause == 1;
    }
};

class SnakeLogic : public GameLogic {
    Snake snake; // Объект с логикой Змейки
public:
    void updateState() override {
        snake.updateState();
    }

    void handleInput(UserAction_t action) override {
        snake.handleInput(action);
    }

    void render(QPainter& painter) override {
        // Логика отрисовки для Змейки
    }

    bool isGameOver() const override {
        return snake.isGameOver();
    }

    bool isPaused() const override {
        return snake.isPaused();
    }
};