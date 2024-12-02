#include "BrickGame.h"
#include <QMessageBox>

#include <QDebug>
#include <QApplication>

BrickGame::BrickGame(QWidget* parent)
    : QWidget(parent) {
    setFixedSize(510, 580);
    // Устанавливаем основной макет
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    // Игровое поле в рамке (слева)
    gameBoardFrame = new QFrame(this);
    gameBoardFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    gameBoardFrame->setLineWidth(2);
    QVBoxLayout *gameLayout = new QVBoxLayout();
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
    speedLabel = new QLabel("Speed: 1", this);
    statsLayout->addWidget(speedLabel);

    // Поле для отображения следующего тетрамино
    nextTetrominoFrame = new QFrame(this);
    nextTetrominoFrame->setFixedSize(100, 100); // Задаем размер для поля 4x4
    nextTetrominoFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    nextTetrominoFrame->setLineWidth(2);
    statsLayout->addWidget(nextTetrominoFrame);
    statsLayout->addStretch();  // Добавляем растяжение, чтобы выровнять статистику
    mainLayout->addWidget(statsFrame);  // Добавляем статистику в основной макет
    // Устанавливаем пропорции ширины
    mainLayout->setStretch(0, 3); // Левый frame (игровое поле) - занимает 3 части
    mainLayout->setStretch(1, 2); // Правый frame (панель статистики) - занимает 1 часть

    setLayout(mainLayout);
}

void BrickGame::keyPressEvent(QKeyEvent* event) {
    emit keyPressed(event); // Эмитируем сигнал без обработки
}

void BrickGame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // Проверяем состояние игры
    if (!isGameStarted) drawStartScreen(painter);
    else if (currentGameInfo.pause) drawPausedScreen(painter);
    else {
        drawGameField(painter, currentGameInfo);
        drawNextTetramino(painter, currentGameInfo);
    }
}

void BrickGame::drawStartScreen(QPainter& painter) {
    // Вычисляем область для текста
    QRect frameRect = gameBoardFrame->geometry();
    QRect textRect(frameRect.x(), frameRect.y(), frameRect.width(), frameRect.height());

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(textRect, Qt::AlignCenter, "For Start press Enter");
}

void BrickGame::drawPausedScreen(QPainter& painter) {
    // Вычисляем область для текста
    QRect frameRect = gameBoardFrame->geometry();
    QRect textRect(frameRect.x(), frameRect.y(), frameRect.width(), frameRect.height());

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(textRect, Qt::AlignCenter, "Paused");
}

void BrickGame::drawGameField(QPainter& painter, const GameInfo_t& game_info) {
    int cellSize = gameBoardFrame->width() / COLS_MAP;
    int xOffset = gameBoardFrame->geometry().x();
    int yOffset = gameBoardFrame->geometry().y();

    painter.setPen(Qt::black);

    for (int row = 0; row < ROWS_MAP; ++row) {
        for (int col = 0; col < COLS_MAP; ++col) {
            QRect cellRect(xOffset + col * cellSize, yOffset + row * cellSize, cellSize, cellSize);
            painter.drawRect(cellRect);

            if (game_info.field[row][col] == 1) {
                painter.fillRect(cellRect, Qt::blue);
            }

            if (game_info.field[row][col] == 2) {
                painter.fillRect(cellRect, Qt::green);
            }
        }
    }
}

void BrickGame::drawNextTetramino(QPainter& painter, const GameInfo_t& game_info) {
    if (!nextTetrominoFrame) return;

    QRect frameRect = nextTetrominoFrame->geometry();  // Границы фрейма
    QRect secondRect = statsFrame->geometry();  // Границы фрейма
    int miniCellSize = frameRect.width() / 4;          // Размер ячейки в поле 4x4

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            QRect cellRect(
                frameRect.x() + secondRect.x() + col * miniCellSize,
                frameRect.y() + secondRect.y() + row * miniCellSize,
                miniCellSize,
                miniCellSize);
            painter.drawRect(cellRect);
            
            if (game_info.next[row][col]) {
                painter.fillRect(cellRect, Qt::green);
            }
        }
    }
}

void BrickGame::updateGame(const GameInfo_t& game_info, State_t state) {
    qDebug() << "really update game";
    setGameInfo(game_info);
    // Обновляем статистику
    updateStats(game_info);

    // Проверяем на конец игры или выход
    // if (state == GAMEOVER) {
    //     showGameOver(game_info);
    // } 
    // else 
    if (state == EXIT_STATE) {
        QApplication::quit();
    }

    update();  // Перерисовка игрового поля
}

void BrickGame::updateStats(const GameInfo_t& game_info) {
    highScoreLabel->setText(QString("High Score: %5").arg(game_info.high_score));
    scoreLabel->setText(QString("Score: %5").arg(game_info.score));
    levelLabel->setText(QString("Level: %2").arg(game_info.level));
    speedLabel->setText(QString("Speed: %2").arg(game_info.speed));
}

void BrickGame::showGameOver(const GameInfo_t& game_info) {
    QMessageBox gameOverBox(this);
    gameOverBox.setWindowTitle("Game Over");
    gameOverBox.setText("Game Over\nYour score: " + QString::number(game_info.score));
    gameOverBox.setIcon(QMessageBox::Information);
    gameOverBox.addButton("Exit", QMessageBox::RejectRole);

    if (gameOverBox.exec() == QMessageBox::RejectRole) {
        close();
        QApplication::quit();
    }
}

void BrickGame::showGameWin(const GameInfo_t& game_info) {
    QMessageBox gameOverBox(this);
    gameOverBox.setWindowTitle("Game Over");
    gameOverBox.setText("Game Over\nYour score: " + QString::number(game_info.score));
    gameOverBox.setIcon(QMessageBox::Information);
    gameOverBox.addButton("Exit", QMessageBox::RejectRole);

    if (gameOverBox.exec() == QMessageBox::RejectRole) {
        close();
        QApplication::quit();
    }
}
