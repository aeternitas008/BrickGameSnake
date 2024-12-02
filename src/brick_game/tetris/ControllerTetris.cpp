#include "ControllerTetris.h"

void TetrisController::GameLoop() {

  Tetris tetris;
  GameInfo_t game_info;
  Tetris_t tetris_info;

  view->DisplayOverlay();
  while (no_break) {
      game_info = tetris.updateCurrentState();
      tetris_info = tetris.GetTetrisInfo();

      UserAction_t signal = UserAction_t::Nosig;
      if (tetris_info.state == MOVING || tetris_info.state == START) {
          int key = GET_USER_INPUT;
          signal = input_handler->GetSignal(key);  // Получаем сигнал через интерфейс ввода
      }

      if (tetris_info.state == START && signal == UserAction_t::Start)
          view->DisplayField();

      if (game_info.pause == 1) {
          view->DisplayPause();
          int key = GET_USER_INPUT;
          input_handler->GetSignal(key);
          if (signal == UserAction_t::Pause) {
              view->ClearPause();
              tetris.GamePause();
          }
      }

      if (game_info.pause != 1) {
          tetris.userInput(signal, signal == Down);
      }

      game_info = tetris.updateCurrentState();

      if (tetris_info.state != START) {
          view->UpdateView(game_info);
      }

      // if (game_info.score == 200) {
      //     view->DisplayGameWin();
      //     no_break = false;
      // }

      if (tetris_info.state == GAMEOVER) {
          view->DisplayGameOver(game_info);
          no_break = false;
      }

      if (tetris_info.state == EXIT_STATE) {
          no_break = false;
      }
  }

}