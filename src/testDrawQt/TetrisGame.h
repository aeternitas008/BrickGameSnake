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

class TetrisGame : public QWidget {
    Q_OBJECT

public:
    explicit TetrisGame(QWidget *parent = nullptr);

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

    // Переменные для игры
    int highScore;
    int score;
    int level;
    int speed;
    int currentPieceX;  // Координата X текущей фигуры
    int currentPieceY;  // Координата Y текущей фигуры
    int gameField[20][10]; // Игровое поле

    void movePieceLeft();
    void movePieceRight();
    void movePieceDown();
};

#endif // TETRISGAME_H
