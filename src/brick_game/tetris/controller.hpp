#ifndef CONTROLLER_TETRIS_H
#define CONTROLLER_TETRIS_H

#include "model.hpp"
#include "../../gui/cli/ConsoleView.h"

// void userInput(UserAction_t action, bool hold);
// UserAction_t GetSignal(int user_input);
void GameLoopTetris(ViewInterface *view);

#endif // CONTROLLER_TETRIS_H