#include "BrickGame.h"
#include <QMessageBox>

#include <QDebug>
#include <QApplication>

BrickGame::BrickGame(Tetris& tetrisInstance, QWidget* parent)
    : QWidget(parent), tetris(tetrisInstance) {
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
    // nextTetrominoFrame->setAlignment(Qt::AlignCenter);
    statsLayout->addWidget(nextTetrominoFrame);
    statsLayout->addStretch();  // Добавляем растяжение, чтобы выровнять статистику
    mainLayout->addWidget(statsFrame);  // Добавляем статистику в основной макет
    // Устанавливаем пропорции ширины
    mainLayout->setStretch(0, 3); // Левый frame (игровое поле) - занимает 3 части
    mainLayout->setStretch(1, 2); // Правый frame (панель статистики) - занимает 1 часть

    setLayout(mainLayout);
    // Создание таймера для обновления состояния игры
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &BrickGame::updateGame);
    gameTimer->start(100);  // Начальная скорость (500 мс)
}

void BrickGame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);


    // QPainter painter(this);

    // // Перерисовываем только измененные области
    // QRect regionToUpdate = event->region().boundingRect();
    // int startRow = regionToUpdate.y() / cellSize;
    // int endRow = (regionToUpdate.y() + regionToUpdate.height()) / cellSize;

    // for (int row = startRow; row <= endRow; ++row) {
    //     for (int col = 0; col < COLS_MAP; ++col) {
    //         QRect cellRect(xOffset + col * cellSize, yOffset + row * cellSize, cellSize, cellSize);
    //         painter.drawRect(cellRect);

    //         if (gameInfo.field[row][col] == 1) {
    //             painter.fillRect(cellRect, Qt::blue);
    //         }
    //     }
    // }



    QPainter painter(this);
    int cellSize = gameBoardFrame->width() / COLS_MAP;  // Размер ячейки
    int xOffset = gameBoardFrame->geometry().x();  // Смещение игрового поля по X
    int yOffset = gameBoardFrame->geometry().y();  // Смещение игрового поля по Y

    painter.setPen(Qt::black);

    // Получаем текущее состояние игрового поля
    auto game_info = tetris.updateCurrentState();
    auto tetramino = tetris.GetTetramino();

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
        if (tetramino.figure[x][y] == 1) {
            game_info.field[tetramino.point->x + x]
                            [tetramino.point->y + y] = 1;
        }
        }
    }

    // Рисуем игровое поле внутри рамки
    for (int row = 0; row < ROWS_MAP; ++row) {
        for (int col = 0; col < COLS_MAP; ++col) {
            QRect cellRect(xOffset + col * cellSize, yOffset + row * cellSize, cellSize, cellSize);
            painter.drawRect(cellRect);

            // Заполняем ячейки, если там есть блок
            if (game_info.field[row][col] == 1) {
                painter.fillRect(cellRect, Qt::blue);
            }
        }
    }

    // Отрисовка следующего тетромино
    if (nextTetrominoFrame) {
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
                if (game_info.next[row][col] == 1) {
                    painter.fillRect(cellRect, Qt::green);
                }
            }
        }
    }
}

void BrickGame::updateGame() {
    Tetris_t tetris_info = tetris.GetTetrisInfo();
    State_t state = tetris_info.state;
    
    qDebug() << "really update game";
    // Проверяем на паузу
    GameInfo_t game_info = Tetris::updateCurrentState();
    if (game_info.pause == 1) {
        // Логика отображения паузы
        return;
    }
    
    // Обновляем отображение и статистику
    updateStats();
    update();  // Перерисовка игрового поля

    // Проверяем на конец игры или выход
    if (state == GAMEOVER) {
        // Логика отображения "Game Over"
        gameTimer->stop();  // Останавливаем таймер
        showGameOver();
    } else if (state == EXIT_STATE) {
        gameTimer->stop();  // Останавливаем игру
        // quit(0);
    }
}

void BrickGame::drawField(GameInfo_t game_info) {
    // QPainter painter(gameBoard);
    // int cellSize = gameBoardFrame->width() / COLS_MAP;

    // for (size_t row = 0; row < ROWS_MAP; ++row) {
    //     for (size_t col = 0; col < COLS_MAP; ++col) {
    //         QRect cellRect(col * cellSize, row * cellSize, cellSize, cellSize);
    //         painter.drawRect(cellRect);
    //         if (game_info.field[row][col] == 1) {
    //             painter.fillRect(cellRect, Qt::blue);
    //         }
    //     }
    // }
    // painter.end();
    // updateStats();
}

void BrickGame::updateStats() {
    GameInfo_t gameInfo = Tetris::updateCurrentState();

    highScoreLabel->setText(QString("High Score: %5").arg(gameInfo.high_score));
    scoreLabel->setText(QString("Score: %5").arg(gameInfo.score));
    levelLabel->setText(QString("Level: %2").arg(gameInfo.level));
    speedLabel->setText(QString("Speed: %2").arg(gameInfo.speed));

    qDebug() << "really update stats";
}

void restartGame() {
    return;
}

void BrickGame::showGameOver() {
    // Создаем сообщение о конце игры
    QMessageBox gameOverBox(this);
    gameOverBox.setWindowTitle("Game Over");
    gameOverBox.setText("Game Over\nYour score: " + scoreLabel->text());
    gameOverBox.setIcon(QMessageBox::Information);

    // Добавляем кнопки
    gameOverBox.addButton("Restart", QMessageBox::AcceptRole);
    gameOverBox.addButton("Exit", QMessageBox::RejectRole);

    // Обрабатываем ответ пользователя
    if (gameOverBox.exec() == QMessageBox::AcceptRole) {
        // Перезапуск игры
        restartGame();
        QApplication::quit();
    } else {
        // Закрытие игры
        close();
        QApplication::quit();
    }
}