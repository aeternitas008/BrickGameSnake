#ifndef CONTROLLER_SNAKE_H
#define CONTROLLER_SNAKE_H

#include "model.hpp"
#include "../../gui/cli/ConsoleView.h"
// void userInput(UserAction_t action, bool hold);
// UserAction_t GetSignal(int user_input);
void GameLoopSnake(ViewInterface *view);

#endif // CONTROLLER_SNAKE_H