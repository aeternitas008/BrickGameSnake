#include "QtSnakeContr.h"
#include <QDebug>
#include <QApplication>

SnakeController::SnakeController(BrickGame* view, Snake& snakeInstance, QObject* parent)
    : QObject(parent), view(view), snake(snakeInstance), inputHandler(new InputHandler()) {
    // Инициализация таймеров
    gameTimer = new QTimer(this);

    // Связываем таймеры и ввод
    connect(gameTimer, &QTimer::timeout, this, &SnakeController::updateGame);
    connect(view, &BrickGame::keyPressed, this, &SnakeController::processInput);
}

SnakeController::~SnakeController() {
    delete inputHandler;
}

void SnakeController::handleKeyPress(QKeyEvent* event) {
    if (!event) return;

    // Если игра ещё не началась
    if (!isGameStarted) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            gameTimer->start(150);  // Устанавливаем таймер для игры змейка
            isGameStarted = true;
            view->isGameStarted = true;
            updateView();
        }
    }

    // Пауза
    if (event->key() == Qt::Key_P) {
        pauseGame();
    }

    if (!isPaused) {
        // Обрабатываем пользовательский ввод
        UserAction_t action = inputHandler->GetSignal(event->key());
        int hold = (action == Down) ? 1 : 0;

        // Передаём действие в модель
        snake.userInput(action, hold);

        qDebug() << "User action in SnakeController: " << action;
        updateView();
    }
}

void SnakeController::processInput(QKeyEvent* event) {
    if (!event) return;
    handleKeyPress(event);
}

void SnakeController::pauseGame() {
    isPaused = !isPaused;

    snake.GamePause();  // Уведомляем модель о паузе

    if (isPaused) {
        gameTimer->stop();
        view->isPaused = true;
    } else {
        gameTimer->start();
        view->isPaused = false;
    }
    updateView();
}

void SnakeController::updateGame() {
    // Получаем текущее состояние игры
    GameInfo_t gameInfo = snake.updateCurrentState();
    SnakeInfo_t snakeInfo = snake.GetSnakeInfo();

    snake.userInput(Nosig, 0);
    // Проверяем конец игры
    if (snakeInfo.state == GAMEOVER || snakeInfo.state == EXIT_STATE || gameInfo.score == 200) {
        gameTimer->stop();  // Останавливаем таймер
        if (snakeInfo.state == GAMEOVER) {
            view->showGameOver(gameInfo);
        } else if (gameInfo.score == 200) {
            view->showGameWin(gameInfo);
        }
        QApplication::quit();  // Выходим из приложения
        return;
    }
    updateView();
    // Обновляем отображение
    updateView();
}

void SnakeController::updateView() {
    // Получаем текущее состояние игры и обновляем представление
    GameInfo_t gameInfo = snake.updateCurrentState();
    view->updateGame(gameInfo, snake.GetSnakeInfo().state);

    qDebug() << "View updated for SnakeController.";
}