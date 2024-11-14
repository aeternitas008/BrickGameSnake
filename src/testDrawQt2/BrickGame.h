#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPainter>
#include <QKeyEvent>
#include "../brick_game/tetris/ModelTetris.h"  // Подключаем вашу модель Tetris
#include "../brick_game/tetris/ControllerTetris.h"

class BrickGame : public QWidget {
    Q_OBJECT

public:
    explicit BrickGame(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    QFrame *gameBoardFrame;
    QWidget *gameBoard;
    QFrame *statsFrame;
    QLabel *highScoreLabel;
    QLabel *scoreLabel;
    QLabel *levelLabel;
    QLabel *speedLabel;
    QTimer *gameTimer;
    Tetris tetris;  // Инстанс вашей игры

    void updateStats();  // Обновление статистики на экране
};

#endif // TETRISGAME_H
