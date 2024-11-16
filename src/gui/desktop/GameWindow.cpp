#include "GameWindow.hpp"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent) : QWidget(parent), gameView(view) {
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

void GameWindow::setView(QtView *view) {
    connect(view, &QtView::UpdateGameInfo, this, &GameWindow::onUpdateGameInfo);
    connect(view, &QtView::ShowPause, this, &GameWindow::onShowPause);
    connect(view, &QtView::HidePause, this, &GameWindow::onHidePause);
    connect(view, &QtView::ShowGameOver, this, &GameWindow::onShowGameOver);
    connect(view, &QtView::ShowGameWin, this, &GameWindow::onShowGameWin);
}

void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    // Отрисовка игрового поля
    for (int x = 0; x < ROWS_MAP; ++x) {
        for (int y = 0; y < COLS_MAP; ++y) {
            if (currentGameInfo.field[x][y] == 1) {
                painter.setBrush(Qt::blue);  // Установим цвет для блоков
                painter.drawRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }

    // Отображение паузы
    if (isPaused) {
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Pause");
    }
}

void GameWindow::onUpdateGameInfo(GameInfo_t game_info) {
    currentGameInfo = gameInfo;
    update();  // Перерисовать окно
}

void GameWindow::onShowPause() {
    isPaused = true;
    update();
}

void GameWindow::onHidePause() {
    isPaused = false;
    update();
}

void GameWindow::onShowGameOver(GameInfo_t gameInfo) {
    currentGameInfo = gameInfo;
    update();
    // Окно с результатом игры
    QMessageBox::information(this, "Game Over", "Game Over! Your score: " + QString::number(currentGameInfo.score));
}

void GameWindow::onShowGameWin() {
    update();
    // Окно с победой
    QMessageBox::information(this, "Game Win", "Congratulations! You won the game.");
}

// Обработка ввода с клавиатуры
void GameWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_P) {
        // Пауза или продолжение игры
        if (isPaused) {
            emit onHidePause();
        } else {
            emit onShowPause();
        }
    }
    else if (event->key() == Qt::Key_Left) {
        // Двигаем фигуру влево
        // controller->moveLeft();  // если у вас есть контроллер
    }
    else if (event->key() == Qt::Key_Right) {
        // Двигаем фигуру вправо
        // controller->moveRight();  // если у вас есть контроллер
    }
    else if (event->key() == Qt::Key_Down) {
        // Двигаем фигуру вниз
        // controller->moveDown();  // если у вас есть контроллер
    }
    else if (event->key() == Qt::Key_Space) {
        // Поворот фигуры
        // controller->rotate();  // если у вас есть контроллер
    }

    QWidget::keyPressEvent(event);
}
