#ifndef BRICKGAME_H
#define BRICKGAME_H

#include <QApplication>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include "../../brick_game/Game.h"
#include "../../brick_game/snake/ModelSnake.h"
#include "../../brick_game/InputHandler.h"

class BrickGame : public QWidget {
    Q_OBJECT

public:
    explicit BrickGame(Game& game, QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    // Логика игры
    Game& game;
    InputHandler* inputHandler;
    QTimer* gameTimer;

    // Состояние игры
    bool isGameStarted = false;
    bool isPaused = false;

    // Интерфейс
    QFrame* gameBoardFrame;
    QFrame* statsFrame;
    QLabel* highScoreLabel;
    QLabel* scoreLabel;
    QLabel* levelLabel;
    QLabel* speedLabel;
    QFrame* nextTetrominoFrame;

    QRect gameBoardRect;
    QRect nextTetrominoRect;

    // Вспомогательные функции
    void updateGame();
    void updateStats(const GameInfo_t& game_info);
    void drawStartScreen(QPainter& painter);
    void drawPausedScreen(QPainter& painter);
    void drawGameField(QPainter& painter, const GameInfo_t& game_info);
    void drawNextTetramino(QPainter& painter, const GameInfo_t& game_info);
    void pauseGame();
    void showGameOver(const GameInfo_t& game_info);
    void showGameWin(const GameInfo_t& game_info);
    void showMessage(const QString& title, const QString& message);
    void updateGeometryCache() { nextTetrominoRect = nextTetrominoFrame->geometry(); }
};

#endif // BRICKGAME_H