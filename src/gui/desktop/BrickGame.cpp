#include "BrickGame.h"

BrickGame::BrickGame(Game& game, QWidget* parent)
    : QWidget(parent), game(game), inputHandler(new InputHandler()) {
    setFixedSize(510, 580);
    setFocusPolicy(Qt::StrongFocus);
    // Устанавливаем основной макет
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // Панель для игрового поля
    gameBoardFrame = new QFrame(this);
    gameBoardFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    gameBoardFrame->setLineWidth(2);
    mainLayout->addWidget(gameBoardFrame);

    // Панель статистики
    statsFrame = new QFrame(this);
    statsFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    statsFrame->setLineWidth(2);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsFrame);

    highScoreLabel = new QLabel("High Score: 0", this);
    statsLayout->addWidget(highScoreLabel);
    scoreLabel = new QLabel("Score: 0", this);
    statsLayout->addWidget(scoreLabel);
    levelLabel = new QLabel("Level: 1", this);
    statsLayout->addWidget(levelLabel);
    speedLabel = new QLabel("Speed: 1", this);
    statsLayout->addWidget(speedLabel);

    nextTetrominoFrame = new QFrame(this);
    nextTetrominoFrame->setFixedSize(100, 100);
    nextTetrominoFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    nextTetrominoFrame->setLineWidth(2);
    statsLayout->addWidget(nextTetrominoFrame);
    statsLayout->addStretch();

    mainLayout->addWidget(statsFrame);
    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 2);

    setLayout(mainLayout);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &BrickGame::updateGame);

    updateGeometryCache();
    gameTimer->setInterval(150);
    gameTimer->start();
}

void BrickGame::pauseGame() {
    isPaused = !isPaused;
    if (isPaused) {
        gameTimer->stop();
    } else {
        gameTimer->start();
    }
    update();
}

void BrickGame::keyPressEvent(QKeyEvent* event) {
    if (!isGameStarted) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            isGameStarted = true;
        }
    }
    if (event->key() == Qt::Key_P) {
        pauseGame();
    }
    if (!isPaused) {
        UserAction_t action = inputHandler->GetSignal(event->key());
        int hold = (action == Down) ? 1 : 0;
        qDebug() << "debug" << action;
        game.userInput(action, hold);
    }
}

void BrickGame::updateGame() {
    game.userInput(Nosig, 0);
    GameInfo_t game_info = game.updateCurrentState();

    if (game_info.state == GAMEOVER || game_info.state == EXIT_STATE || 
       (game_info.score == 200 && dynamic_cast<Snake*>(&game))) {
        gameTimer->stop();
        if (game_info.state == GAMEOVER) {
            showGameOver(game_info);
        } else if (game_info.score == 200 && dynamic_cast<Snake*>(&game)) {
            showGameWin(game_info);
        }
        QApplication::quit();
        return;
    }

    updateStats(game_info);
    update();
}

void BrickGame::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);

    gameBoardRect = gameBoardFrame->geometry();

    if (!isGameStarted) {
        drawStartScreen(painter);
    } else if (isPaused) {
        drawPausedScreen(painter);
    } else {
        GameInfo_t game_info = game.updateCurrentState();
        drawGameField(painter, game_info);
        drawNextTetramino(painter, game_info);
    }
}

void BrickGame::updateStats(const GameInfo_t& game_info) {
    highScoreLabel->setText(QString("High Score: %5").arg(game_info.high_score));
    scoreLabel->setText(QString("Score: %5").arg(game_info.score));
    levelLabel->setText(QString("Level: %2").arg(game_info.level));
    speedLabel->setText(QString("Speed: %2").arg(game_info.speed));
}

void BrickGame::showGameOver(const GameInfo_t& game_info) {
    showMessage("Game Over", "Game Over\nYour score: " + QString::number(game_info.score));
}

void BrickGame::showGameWin(const GameInfo_t& game_info) {
    showMessage("You Win!", "Congratulations!\nYour score: " + QString::number(game_info.score));
}

void BrickGame::showMessage(const QString& title, const QString& message) {
    QMessageBox messageBox(this);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.addButton("Exit", QMessageBox::RejectRole);

    if (messageBox.exec() == QMessageBox::RejectRole) {
        close();
        QApplication::quit();
    }
}

void BrickGame::drawPausedScreen(QPainter& painter) {
    // Вычисляем область для текста
    QRect textRect(gameBoardRect.x(), gameBoardRect.y(), gameBoardRect.width(), gameBoardRect.height());

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(gameBoardRect, Qt::AlignCenter, "Paused");
}

void BrickGame::drawGameField(QPainter& painter, const GameInfo_t& game_info) {
    int cellSize = gameBoardFrame->width() / COLS_MAP;
    int xOffset = gameBoardRect.x();
    int yOffset = gameBoardRect.y();

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

    QRect secondRect = statsFrame->geometry();  // Границы фрейма
    int miniCellSize = nextTetrominoRect.width() / 4;          // Размер ячейки в поле 4x4

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            QRect cellRect(
                nextTetrominoFrame->x() + secondRect.x() + col * miniCellSize,
                nextTetrominoFrame->y() + secondRect.y() + row * miniCellSize,
                miniCellSize,
                miniCellSize);
            painter.drawRect(cellRect);
            
            if (game_info.next[row][col]) {
                painter.fillRect(cellRect, Qt::green);
            }
        }
    }
}

void BrickGame::drawStartScreen(QPainter& painter) {
    // Вычисляем область для текста
    QRect textRect(gameBoardRect.x(), gameBoardRect.y(), gameBoardRect.width(), gameBoardRect.height());

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(textRect, Qt::AlignCenter, "For Start press Enter");
}