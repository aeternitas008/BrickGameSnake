#include "BrickGame.h"
#include <QMessageBox>

BrickGame::BrickGame(Tetris& tetrisInstance, QWidget* parent)
    : QWidget(parent), tetris(tetrisInstance) {
    setFixedSize(600, 500);
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
    // Создание таймера для обновления состояния игры
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &BrickGame::updateGame);
    gameTimer->start(500);  // Начальная скорость (500 мс)
}

void BrickGame::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(gameBoard);
    int cellSize = gameBoard->width() / COLS_MAP;  // Размер ячейки (количество колонок)

    painter.setPen(Qt::black);

    // Получаем текущее состояние игрового поля
    auto gameState = Tetris::updateCurrentState();
    
    // Рисуем игровое поле
    for (int row = 0; row < ROWS_MAP; ++row) {
        for (int col = 0; col < COLS_MAP; ++col) {
            QRect cellRect(col * cellSize, row * cellSize, cellSize, cellSize);
            painter.drawRect(cellRect);

            // Заполняем ячейки, если там есть блок
            if (gameState.field[row][col] == 1) {
                painter.fillRect(cellRect, Qt::blue);
            }
        }
    }
}

void BrickGame::updateGame() {
    Tetris_t tetris_info = tetris.GetTetrisInfo();
    State_t state = tetris_info.state;

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
    }
}

// void BrickGame::drawField(const std::vector<std::vector<int>>& field, const Tetramino_t& currentTetramino) {
//     QPainter painter(gameBoard);
//     int cellSize = gameBoard->width() / field[0].size();

//     for (size_t row = 0; row < field.size(); ++row) {
//         for (size_t col = 0; col < field[row].size(); ++col) {
//             QRect cellRect(col * cellSize, row * cellSize, cellSize, cellSize);
//             painter.drawRect(cellRect);
//             if (field[row][col] == 1) {
//                 painter.fillRect(cellRect, Qt::blue);
//             }
//         }
//     }

//     for (const auto& point : currentTetramino.point) {
//         QRect pieceRect(point.x * cellSize, point.y * cellSize, cellSize, cellSize);
//         painter.fillRect(pieceRect, Qt::red);
//     }
// }


void BrickGame::drawField(GameInfo_t game_info) {
    updateStats();
    QPainter painter(gameBoard);
    int cellSize = gameBoard->width() / COLS_MAP;

    for (size_t row = 0; row < ROWS_MAP; ++row) {
        for (size_t col = 0; col < COLS_MAP; ++col) {
            QRect cellRect(col * cellSize, row * cellSize, cellSize, cellSize);
            painter.drawRect(cellRect);
            if (game_info.field[row][col] == 1) {
                painter.fillRect(cellRect, Qt::blue);
            }
        }
    }
}

void BrickGame::updateStats() {
    GameInfo_t gameInfo = Tetris::updateCurrentState();

    highScoreLabel->setText(QString("High Score: %1").arg(gameInfo.high_score));
    scoreLabel->setText(QString("Score: %1").arg(gameInfo.score));
    levelLabel->setText(QString("Level: %1").arg(gameInfo.level));
    speedLabel->setText(QString("Speed: %1 ms").arg(gameInfo.speed));
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
    // if (gameOverBox.exec() == QMessageBox::AcceptRole) {
    //     // Перезапуск игры
    //     restartGame();
    // } else {
        // Закрытие игры
    close();
    // }
}