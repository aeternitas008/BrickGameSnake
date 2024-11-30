#include <string>
#include <iostream>
#include "gui/cli/ConsoleView.h"
#include "brick_game/snake/ControllerSnake.h"
#include "brick_game/tetris/ControllerTetris.h"

#include "brick_game/InputHandler.h"

#ifdef USE_QT
    #undef scroll
    #include <QApplication>
    #include "gui/desktop/QtView.h"
    #include "gui/desktop/GameWindow.hpp"
    #include "brick_game/QtInputHandler.h"
#endif

int main(int argc, char *argv[]) {
    // Проверяем, передан ли аргумент для выбора игры
    std::string gameChoice = "snake";  // По умолчанию игра Snake
    if (argc > 1) {
        gameChoice = argv[1];  // Получаем название игры из аргумента
    }
// #ifdef USE_QT
//     QApplication app(argc, argv);
//     GameWindow window;
//     QtInputHandler input;
//     QtView view;
//     window.setView(&view);
//     if (gameChoice == "snake") {
//     // Пример для игры в "Змейку"
//         GameController<Snake> snake_controller(&input, &view);
//         window.setController(&snakeController);  // Устанавливаем контроллер
//         QTimer *timer = new QTimer(&window);  // Таймер для обновления игрового цикла
//         QObject::connect(timer, &QTimer::timeout, [&]() {
//             snake_controller.GameLoop();  // Периодически вызываем игровой цикл
//             window.update();  // Обновляем виджет
//         });
//         snake_controller.GameLoop();  // Запускаем цикл игры
//     } else if (gameChoice == "tetris") {
//     // Пример для игры в "Тетрис"
//         GameController<Tetris> tetris_controller(&input, &view);
//         window.setController(&tetrisController);  // Устанавливаем контроллер
//         QTimer *timer = new QTimer(&window);  // Таймер для обновления игрового цикла
//         QObject::connect(timer, &QTimer::timeout, [&]() {
//             tetrisController.GameLoop();  // Запускаем цикл игры
//             window.update();  // Обновляем виджет
//         });
//         timer->start(100);  // Устанавливаем интервал обновления 100 мс (или любое другое значение)
//     } else {
//         std::cerr << "Unknown game: " << gameChoice << "\n";
//         return 1;
//     }
//     return app.exec();
// #else
    InputHandler input;
    ConsoleView view;
    setlocale(LC_CTYPE, "");
    WIN_INIT(INITIAL_TIMEOUT);
    if (gameChoice == "snake") {
        SnakeController snake_controller(&input, &view);
        snake_controller.GameLoop();  // Запускаем цикл игры
    } else if (gameChoice == "tetris") {
        TetrisController tetris_controller(&input, &view);
        tetris_controller.GameLoop();  // Запускаем цикл игры
    } else {
        std::cout << "Unknown game: " << gameChoice << "\n";
        return 1;
    }
    endwin();
    return SUCCESS; 
// #endif
}