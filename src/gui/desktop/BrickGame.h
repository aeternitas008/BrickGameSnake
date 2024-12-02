#ifndef TETRISGAME_H
#define TETRISGAME_H

#undef scroll
#include "../../brick_game/tetris/ModelTetris.h"  // Подключаем вашу модель Tetris
#include "../../brick_game/snake/ModelSnake.h"
#include <QWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPainter>
#include <QKeyEvent>

class BrickGame : public QWidget {
    Q_OBJECT

public:
    explicit BrickGame(QWidget* parent = nullptr);
    
    void updateGame(const GameInfo_t& game_info, State_t state);
    void setGameInfo(const GameInfo_t& game_info) {
        currentGameInfo = game_info;
    }
    void showGameOver(const GameInfo_t& game_info);
    void showGameWin(const GameInfo_t& game_info);

    bool isPaused = false;
    bool isGameStarted = false;

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void keyPressed(QKeyEvent* event);


private:
    GameInfo_t currentGameInfo; // Текущая информация о состоянии игры

    void updateStats(const GameInfo_t& game_info);
    void drawGameField(QPainter& painter, const GameInfo_t& game_info);
    void drawNextTetramino(QPainter& painter, const GameInfo_t& game_info);
    void drawPausedScreen(QPainter& painter);
    void drawStartScreen(QPainter& painter);


    QFrame* gameBoardFrame;
    QFrame* statsFrame;
    QFrame* nextTetrominoFrame;

    QLabel* highScoreLabel;
    QLabel* scoreLabel;
    QLabel* levelLabel;
    QLabel* speedLabel;
    QTimer* gameTimer;

};

#endif // TETRISGAME_H
