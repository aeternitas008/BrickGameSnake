#include "QtTetrisContr.h"
#include <QDebug>
#include <QApplication>

TetrisController::TetrisController(BrickGame* view, Tetris& tetrisInstance, QObject* parent)
    : QObject(parent), view(view), tetris(tetrisInstance), inputHandler(new InputHandler()) {
    gameTimer = new QTimer(this);
    qDebug() << "yes";
    // Связываем таймер с обновлением игры
    connect(gameTimer, &QTimer::timeout, this, &TetrisController::updateGame);
    // Связываем сигнал нажатия клавиш с обработчиком ввода
    connect(view, &BrickGame::keyPressed, this, &TetrisController::processInput);
}

TetrisController::~TetrisController() {
    delete inputHandler;
}

void TetrisController::handleKeyPress(QKeyEvent* event) {
    if (!event) return;
    qDebug() << "i am here" << event->key();
    // Если игра не началась
    if (!isGameStarted) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
          gameTimer->start(107);
          isGameStarted = true;
          view->isGameStarted = true;
          updateView();            // Отображение начального состояния
        }
    }

    if (event->key() == Qt::Key_P) {
        pauseGame();  // Снимаем или ставим игру на паузу
    }

    if (!isPaused) {
        // Обработка игрового ввода
        UserAction_t action = inputHandler->GetSignal(event->key());
        int hold = (action == Down) ? 1 : 0;

        // Передаем действие в модель и обновляем отображение
        tetris.userInput(action, hold);
        updateView();
    }
}

void TetrisController::processInput(QKeyEvent* event) {
    if (!event) return;
    handleKeyPress(event);
    qDebug() << "Processing user input: " << event->key();
}

void TetrisController::pauseGame() {
    isPaused = !isPaused;

    tetris.GamePause();     // Уведомляем модель о паузе

    if (isPaused) {
        gameTimer->stop();  // Останавливаем таймер
        view->isPaused = true;
    } else {
        gameTimer->start(); // Возобновляем таймер
        view->isPaused = false;
    }
    updateView();           // Обновляем отображение
}

void TetrisController::updateGame() {
    // Получаем состояние игры
    GameInfo_t gameInfo = tetris.updateCurrentState();
    Tetris_t tetrisInfo = tetris.GetTetrisInfo();
    tetris.userInput(Nosig, 0);
    // Проверяем состояние игры
    if (tetrisInfo.state == GAMEOVER || tetrisInfo.state == EXIT_STATE) {
        gameTimer->stop();  // Останавливаем таймер
        if (tetrisInfo.state == GAMEOVER) 
            view->showGameOver(gameInfo);  // Показываем "игра окончена"
        QApplication::quit();  // Выходим из приложения
        return;
    }
    updateView();
}

void TetrisController::updateView() {
    GameInfo_t gameInfo = tetris.updateCurrentState();
    view->updateGame(gameInfo, tetris.GetTetrisInfo().state);
}