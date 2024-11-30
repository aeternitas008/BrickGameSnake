#include <QTimer>
#include "TetrisView.h"
#include "TetrisController.h"

TetrisView::TetrisView(Tetris* tetrisModel, QWidget* parent)
    : QWidget(parent), tetris(tetrisModel) {
    controller = new TetrisController(tetrisModel, this);
    
    // Создаем таймер для игрового цикла
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &TetrisView::gameLoop);
    gameTimer->start(1000 / 60);  // 60 FPS
}

void TetrisView::gameLoop() {
    // Получаем текущее состояние игры
    GameInfo_t game_info = tetris->updateCurrentState();
    Tetris_t tetris_info = tetris->GetTetrisInfo();
    
    // Обрабатываем состояние игры
    if (tetris_info.state == GAMEOVER) {
        displayGameOver();
        gameTimer->stop();  // Останавливаем игру
    } else if (tetris_info.state == EXIT_STATE) {
        close();  // Закрываем окно
    } else {
        Tetramino_t tetramino = tetris->GetTetramino();
        SumTetris(&game_info, tetramino);
        updateView(game_info);  // Обновляем отображение
        displayNextTetramino(game_info);  // Показ следующего тетромино
    }
}

void TetrisView::displayGameOver() {
    // Логика отображения "Game Over"
}

void TetrisView::updateView(const GameInfo_t& game_info) {
    // Логика перерисовки поля игры
}

void TetrisView::displayNextTetramino(const GameInfo_t& game_info) {
    // Логика отображения следующего тетромино
}
