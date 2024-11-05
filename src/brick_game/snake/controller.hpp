#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.hpp"
#include "../../gui/cli/view.hpp"
void userInput(UserAction_t action, bool hold);
UserAction_t GetSignal(int user_input);
void GameLoop();

#endif // CONTROLLER_H