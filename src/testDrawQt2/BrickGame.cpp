#include "BrickGame.h"

BrickGame::BrickGame(QWidget *parent)
    : QWidget(parent), tetris() {
    setFixedSize(600, 500);  // Устанавливаем фиксированный размер окна

    // Создание таймера для обновления состояния игры
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &BrickGame::updateGame);
    gameTimer->start(500);  // Начальная скорость (500 мс)

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

    // bool no_break = TRUE;
    // Tetris tetris;
    // GameInfo_t game_info;
    // Tetris_t tetris_info;
    // Tetramino_t tetramino;
    // State_t state;
    // int signal = 0;

    // tetris.StartGame();  // Запуск игры
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

    // Отрисовка текущего тетромино
    Tetramino_t currentTetramino = tetris.GetTetramino();
    for (int i = 0; i < 4; ++i) {
        int x = currentTetramino.point[i].x;
        int y = currentTetramino.point[i].y;
        QRect pieceRect(x * cellSize, y * cellSize, cellSize, cellSize);
        painter.fillRect(pieceRect, Qt::red);
    }
}

// void BrickGame::userInput(UserAction_t signal, bool hold) {
//   Tetris tetris;
//   Tetris_t tetris_info = tetris.GetTetrisInfo();
//   tetris.SetHold(hold);
//   action fsm_simple[6] = { nullptr, &Tetris::Spawn, nullptr, &Tetris::Shifting, &Tetris::GameOver, &Tetris::ExitState};
//   action fsm_table[2][9] = {
//       {&Tetris::StartGame, nullptr, &Tetris::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
//       {nullptr, &Tetris::GamePause, &Tetris::ExitState, &Tetris::MoveLeft, &Tetris::MoveRight, &Tetris::MoveUp, &Tetris::MoveDown, &Tetris::TurnRight, &Tetris::Check},
//   };
//   action act;
//   if (tetris_info.state != MOVING && tetris_info.state != START) {
//     act = fsm_simple[tetris_info.state];
//   } else  {
//     int state = (tetris_info.state == MOVING) ? 1 : 0;
//     act = fsm_table[state][signal];
//   }
//   if (act) {
//     (tetris.*act)();
//   }
// }


void BrickGame::keyPressEvent(QKeyEvent *event) {
    UserAction_t signal = Nosig;

    // Маппинг Qt-клавиш на сигналы консольного контроллера
    switch (event->key()) {
        case Qt::Key_Left:
            signal = Left;
            break;
        case Qt::Key_Right:
            signal = Right;
            break;
        case Qt::Key_Down:
            signal = Down;
            break;
        case Qt::Key_Up:
            signal = Up;
            break;
        case Qt::Key_Enter:
            signal = Start;
            break;
        case Qt::Key_Escape:
            signal = Terminate;
            break;
        case Qt::Key_Space:
            signal = Action;
            break;
        case Qt::Key_P:
            signal = Pause;
            break;
        default:
            signal = Nosig;
            break;
    }

    tetris.userInput(signal, event->key() == Qt::Key_Down);
    update();
}

void BrickGame::updateGame() {
    Tetris_t tetris_info = tetris.GetTetrisInfo();
    State_t state = tetris_info.state;

    if (state == MOVING || state == START) {
        // Здесь можно использовать сигнал для управления действиями
        int signal = 0;  // Здесь должна быть логика получения сигнала, например, если есть очередь событий
        if (signal != 0) {
            userInput(GetSignal(signal), signal == KEY_DOWN);
        }
    }

    // Обновляем состояние игры
    tetris.Shifting();  // Двигаем тетромино

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
    } else if (state == EXIT_STATE) {
        gameTimer->stop();  // Останавливаем игру
    }
}


void BrickGame::updateStats() {
    GameInfo_t gameInfo = Tetris::updateCurrentState();

    highScoreLabel->setText(QString("High Score: %1").arg(gameInfo.high_score));
    scoreLabel->setText(QString("Score: %1").arg(gameInfo.score));
    levelLabel->setText(QString("Level: %1").arg(gameInfo.level));
    speedLabel->setText(QString("Speed: %1 ms").arg(gameInfo.speed));
}
