#include <string>
#include <iostream>
#include "../gui/cli/ConsoleView.h"
#include "../brick_game/snake/ControllerSnake.h"
#include "../brick_game/tetris/ControllerTetris.h"
#include "../brick_game/InputHandler.h"

int main(int argc, char *argv[]) {
    // Проверяем, передан ли аргумент для выбора игры
    std::string gameChoice = "snake";  // По умолчанию игра Snake
    if (argc > 1) {
        gameChoice = argv[1];  // Получаем название игры из аргумента
    }
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
}