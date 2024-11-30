// SnakeController.h
#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H
// распределить по файлам, вынести описание функции
// #include "../../gui/ViewInterface.h"
#include "../InputHandler.h"
#include "ModelSnake.h"
#include "../../gui/cli/ConsoleView.h"

class SnakeController {
public:
    SnakeController(InputHandler* input, ViewInterface* view) 
        : input_handler(input), view(view), no_break(true) {}

    void GameLoop();

private:
    InputHandler* input_handler;  // Источник ввода
    ViewInterface* view;          // Интерфейс отображения
    bool no_break;

    // вынести в бизнес логику? или другие способы
    
};

#endif  // SNAKE_CONTROLLER_H
