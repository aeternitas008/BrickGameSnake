// #include <QApplication>
// #include "gui/desktop/GameWindow.hpp"
// #include "brick_game/snake/controller.hpp"
// #include "brick_game/tetris/controller.hpp"

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     // Выберите игру (например, Snake)
//     bool isSnakeGame = true;

//     GameWindow gameWindow;
    
//     if (isSnakeGame) {
//         SnakeController snakeController;
//         gameWindow.setController(&snakeController);
//     } else {
//         TetrisController tetrisController;
//         gameWindow.setController(&tetrisController);
//     }

//     gameWindow.show();
//     return app.exec();
// }

// main.cpp
#include <string>
#include <iostream>
#include "gui/cli/ConsoleView.h"
#include "brick_game/snake/controller.hpp"
#include "brick_game/tetris/controller.hpp"
#ifdef USE_QT
#include <QApplication>
#include "QtView.h"
#endif

int main(int argc, char *argv[]) {
    // Проверяем, передан ли аргумент для выбора игры
    std::string gameChoice = "snake";  // По умолчанию игра Snake
    if (argc > 1) {
        gameChoice = argv[1];  // Получаем название игры из аргумента
    }
#ifdef USE_QT
    QApplication app(argc, argv);
    QtView view;
    if (gameChoice == "snake") {
        GameLoopSnake(&view);  // Запускаем игру с Qt интерфейсом
    } else if (gameChoice == "tetris") {
        GameLoopTetris(&view);  // Запускаем игру с Qt интерфейсом
    } else {
        std::cerr << "Unknown game: " << gameChoice << "\n";
        return 1;
    }
    return app.exec();
#else
    ConsoleView view;
    setlocale(LC_CTYPE, "");
    WIN_INIT(INITIAL_TIMEOUT);
    if (gameChoice == "snake") {
        GameLoopSnake(&view);  // Запускаем игру с Qt интерфейсом
    } else if (gameChoice == "tetris") {
        GameLoopTetris(&view);  // Запускаем игру с Qt интерфейсом
    } else {
        std::cout << "Unknown game: " << gameChoice << "\n";
        return 1;
    }
    endwin();
    return SUCCESS; 
#endif
}