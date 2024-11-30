#ifndef CONTROLLER_TETRIS_H
#define CONTROLLER_TETRIS_H

#include "ModelTetris.h"
#include "../../gui/cli/ConsoleView.h"
#include "../InputHandler.h"

class TetrisController {
public:
    TetrisController(InputHandler* input, ViewInterface* view) 
        : input_handler(input), view(view), no_break(true) {}

    void GameLoop();

private:
    InputHandler* input_handler;  // Источник ввода
    ViewInterface* view;          // Интерфейс отображения
    bool no_break;
};

#endif // CONTROLLER_TETRIS_H