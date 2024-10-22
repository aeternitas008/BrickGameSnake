#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"
#include "defines.h"

void userInput(UserAction_t action, bool hold);

UserAction_t GetSignal(int user_input);
void SigAct(UserAction_t sig);

void GameLoop();
#endif // CONTROLLER_H