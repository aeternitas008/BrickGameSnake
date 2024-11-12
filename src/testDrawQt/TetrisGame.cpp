#include "TetrisGame.h"

TetrisGame::TetrisGame(QWidget *parent)
    : QWidget(parent), highScore(0), score(0), level(1), speed(500), currentPieceX(5), currentPieceY(0) {
    setFixedSize(600, 500);  // Устанавливаем фиксированный размер окна

    // Инициализируем игровое поле
    memset(gameField, 0, sizeof(gameField));

    // Создание таймера для обновления состояния игры
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &TetrisGame::updateGame);
    gameTimer->start(speed); // Обновляем игру каждые 'speed' миллисекунд

    // Устанавливаем основной макет
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Игровое поле в рамке (слева)
    gameBoardFrame = new QFrame(this);
    gameBoardFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    gameBoardFrame->setLineWidth(2);

    gameBoard = new QWidget(gameBoardFrame);
    gameBoard->setFixedSize(200, 400);

    QVBoxLayout *gameLayout = new QVBoxLayout();
    gameLayout->addWidget(gameBoard);
    gameBoardFrame->setLayout(gameLayout);

    mainLayout->addWidget(gameBoardFrame);

    // Панель статистики в рамке (справа)
    statsFrame = new QFrame(this);
    statsFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    statsFrame->setLineWidth(2);

    QVBoxLayout *statsLayout = new QVBoxLayout(statsFrame);

    // High Score
    highScoreLabel = new QLabel("High Score: 0", this);
    statsLayout->addWidget(highScoreLabel);

    // Score
    scoreLabel = new QLabel("Score: 0", this);
    statsLayout->addWidget(scoreLabel);

    // Level
    levelLabel = new QLabel("Level: 1", this);
    statsLayout->addWidget(levelLabel);

    // Speed
    speedLabel = new QLabel("Speed: 500 ms", this);
    statsLayout->addWidget(speedLabel);

    statsLayout->addStretch();  // Добавляем растяжение, чтобы выровнять статистику

    mainLayout->addWidget(statsFrame);  // Добавляем статистику в основной макет
    setLayout(mainLayout);
}

void TetrisGame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(gameBoard);
    int cellSize = gameBoard->width() / 10;  // Размер ячейки (на основании 10 колонок)

    painter.setPen(Qt::black);

    // Рисуем игровое поле
    for (int row = 0; row < 20; ++row) {
        for (int col = 0; col < 10; ++col) {
            QRect cellRect(col * cellSize, row * cellSize, cellSize, cellSize);
            painter.drawRect(cellRect);

            // Заполняем ячейки, где есть фигура
            if (gameField[row][col] == 1) {
                painter.fillRect(cellRect, Qt::blue);
            }
        }
    }

    // Отрисовка текущей фигуры (для демонстрации фиксированного квадрата)
    QRect pieceRect(currentPieceX * cellSize, currentPieceY * cellSize, cellSize, cellSize);
    painter.fillRect(pieceRect, Qt::red);
}

void TetrisGame::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Left:
            movePieceLeft();
            break;
        case Qt::Key_Right:
            movePieceRight();
            break;
        case Qt::Key_Down:
            movePieceDown();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

void TetrisGame::movePieceLeft() {
    if (currentPieceX > 0) {
        currentPieceX--;
        gameBoard->update();
    }
}

void TetrisGame::movePieceRight() {
    if (currentPieceX < 9) {
        currentPieceX++;
        gameBoard->update();
    }
}

void TetrisGame::movePieceDown() {
    if (currentPieceY < 19) {
        currentPieceY++;
        gameBoard->update();
    }
}

void TetrisGame::updateGame() {
    // Логика автоматического движения фигуры вниз
    movePieceDown();

    // Обновление счёта и уровня
    score += 10;
    if (score > highScore) {
        highScore = score;  // Обновляем рекорд
    }

    if (score % 100 == 0) {
        level++;
        speed = qMax(100, speed - 50);  // Ускоряем игру, но не меньше 100 мс
        gameTimer->setInterval(speed);
    }

    // Обновление меток
    highScoreLabel->setText(QString("High Score: %1").arg(highScore));
    scoreLabel->setText(QString("Score: %1").arg(score));
    levelLabel->setText(QString("Level: %1").arg(level));
    speedLabel->setText(QString("Speed: %1 ms").arg(speed));

    // Перерисовка игрового поля
    gameBoard->update();
}
