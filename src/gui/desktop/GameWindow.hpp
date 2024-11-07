#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include "QtView.h"

class GameWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameWindow(QtView* view, QWidget* parent = nullptr)
        : QWidget(parent), gameView(view) {
        setFixedSize(800, 600);  // Размер окна
        setWindowTitle("Brick Game");

        // Подключаем сигналы из QtView к слотам
        connect(gameView, &QtView::UpdateGameInfo, this, &GameWindow::onUpdateGameInfo);
        connect(gameView, &QtView::ShowPause, this, &GameWindow::onShowPause);
        connect(gameView, &QtView::HidePause, this, &GameWindow::onHidePause);
        connect(gameView, &QtView::ShowGameOver, this, &GameWindow::onShowGameOver);
        connect(gameView, &QtView::ShowGameWin, this, &GameWindow::onShowGameWin);
        connect(gameView, &QtView::PrintOverlay, this, &GameWindow::onPrintOverlay);
        connect(gameView, &QtView::PrintField, this, &GameWindow::onPrintField);
        connect(gameView, &QtView::PrintNextTetramino, this, &GameWindow::onPrintNextTetramino);

        // Таймер для обновления игры
        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &GameWindow::updateGame);
        gameTimer->start(100);  // Таймер обновления с интервалом 100 мс
    }

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
    void onUpdateGameInfo(GameInfo_t game_info) {
        // Обновляем информацию о состоянии игры
        // например, обновление счета, состояния игры
        update();  // Перерисовываем окно
    }

    void onShowPause() {
        // Отображаем экран паузы
        // Можете отобразить виджет с текстом "Пауза"
        QMessageBox::information(this, "Pause", "Game is paused!");
    }

    void onHidePause() {
        // Скрываем экран паузы
        // Возвращаем игру в нормальное состояние
    }

    void onShowGameOver(GameInfo_t game_info) {
        // Отображаем экран "Game Over"
        // Например, выводим сообщение о завершении игры
        QMessageBox::information(this, "Game Over", "Game Over!");
    }

    void onShowGameWin() {
        // Отображаем экран победы
        // Например, выводим сообщение о выигрыше
        QMessageBox::information(this, "Game Win", "You Win!");
    }

    void onPrintOverlay() {
        // Отображаем оверлей (например, кнопки или подсказки)
        // Можете нарисовать элементы интерфейса
    }

    void onPrintField() {
        // Отображаем игровое поле
        update();  // Перерисовываем поле
    }

    void onPrintNextTetramino(GameInfo_t game_info) {
        // Отображаем следующий тетромино для игры в тетрис
        // Нарисуйте тетромино на экране
    }

    void updateGame() {
        // Обновление состояния игры
        gameView->UpdateView(GameInfo_t());  // Например, передаем пустую информацию для примера
        update();  // Перерисовываем экран
    }

private:
    QtView* gameView;  // Представление
    QTimer* gameTimer; // Таймер для обновления игры

    bool isPaused;

    void drawGameField(QPainter& painter) {
        // Пример рисования игрового поля
        painter.setBrush(QBrush(Qt::green));
        painter.drawRect(50, 50, 700, 500); // Нарисуем игровое поле
    }
};
