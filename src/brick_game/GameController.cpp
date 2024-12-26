#include "GameController.h"

void GameController::GameLoop() {
  GameInfo_t game_info;
  view->PrintOverlay();

  while (no_break) {
    game_info = game.updateCurrentState();

    UserAction_t signal = UserAction_t::Nosig;
    if (game_info.state == MOVING || game_info.state == START) {
      int key = GET_USER_INPUT;
      signal = input_handler->GetSignal(key);
    }

    if (game_info.state == START && signal == UserAction_t::Start)
      view->PrintField();

    if (game_info.pause) {
      view->PrintPause();
      if (signal == UserAction_t::Pause) {
        view->ClearPause();
        game.userInput(signal, 0);
      }
    }

    if (game_info.state != START) {
      view->UpdateView(game_info);
    }

    if (game_info.score == 200 && dynamic_cast<Snake*>(&game)) {
      view->PrintGameWin();
      no_break = false;
    }

    if (game_info.state == GAMEOVER) {
      view->PrintGameOver(game_info);
      no_break = false;
    }

    if (game_info.state == EXIT_STATE) {
      no_break = false;
    }

    if (!game_info.pause) {
      int hold = (signal == UserAction_t::Down);
      game.userInput(signal, hold);
    }
  }
}