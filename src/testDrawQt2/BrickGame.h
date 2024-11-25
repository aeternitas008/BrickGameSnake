#ifndef TETRISGAME_H
#define TETRISGAME_H

#undef scroll
#include "../brick_game/tetris/ModelTetris.h"  // Подключаем вашу модель Tetris
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
    explicit BrickGame(Tetris& tetrisInstance, QWidget* parent = nullptr);
    QTimer* getGameTimer() const { return gameTimer; }
    void showGameOver();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void drawGameField(QPainter& painter);
    void drawPausedScreen(QPainter& painter);
    void drawStartScreen(QPainter& painter);

private:
    Tetris& tetris;  // Ссылка на объект Tetris

private slots:
    void updateGame();

signals:
    void keyPressed(QKeyEvent* event);

private:
    QFrame *gameBoardFrame;
    QFrame *statsFrame;
    QFrame *nextTetrominoFrame;

    QLabel *highScoreLabel;
    QLabel *scoreLabel;
    QLabel *levelLabel;
    QLabel *speedLabel;
    QTimer *gameTimer;

    bool isGameStarted = false;  // Флаг состояния игры
    void updateStats();  // Обновление статистики на экране
};

#endif // TETRISGAME_H
