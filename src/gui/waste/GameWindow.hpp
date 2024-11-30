#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include "QtView.h"

class GameWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameWindow(QtView* view, QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override {
        // Передаем нажатие клавиши в представление
        gameView->handleKeyPress(event->key());
    }

    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);

        // Пример рисования игрового поля
        drawGameField(painter);
    }

private slots:
    // Обработчики сигналов
    void onUpdateGameInfo(GameInfo_t game_info);
    void onShowPause();
    void onHidePause();
    void onShowGameOver(GameInfo_t game_info);
    void onShowGameWin();
    void onPrintOverlay();
    void onPrintField();
    void onPrintNextTetramino(GameInfo_t game_info);
    void updateGame();

private:
    QtView* gameView;  // Представление
    QTimer* gameTimer; // Таймер для обновления игры
    Tetris tetris;
    Snake snake;

    void drawGameField(QPainter& painter) {
        // Пример рисования игрового поля
        painter.setBrush(QBrush(Qt::green));
        painter.drawRect(50, 50, 700, 500); // Нарисуем игровое поле
    }
};
