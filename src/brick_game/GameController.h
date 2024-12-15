// GameController.h
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "InputHandler.h"
#include "Game.h"
#include "../gui/cli/ConsoleView.h"
#include "snake/ModelSnake.h"

class GameController {
public:
    GameController(ViewInterface* view, Game& game) 
        : view(view), game(game), no_break(true) {}

    void GameLoop();

private:
    ViewInterface* view;          // Интерфейс отображения
    InputHandler* input_handler;          // Интерфейс отображения
    Game& game;
    bool no_break;
};

#endif  // GAME_CONTROLLER_H
