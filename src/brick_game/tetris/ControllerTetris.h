#ifndef CONTROLLER_TETRIS_H
#define CONTROLLER_TETRIS_H

#include "ModelTetris.h"
#include "../../gui/cli/ConsoleView.h"

// void GameLoopTetris(ViewInterface *view);
// UserAction_t GetSignal(int user_input);

// UserAction_t GetSignal(int user_input) {
//   UserAction_t rc = Nosig;
//   if (user_input == ENTER_KEY)
//     rc = Start;
//   else if (user_input == P_KEY_SMALL || user_input == P_KEY)
//     rc = Pause;
//   else if (user_input == ESCAPE_KEY)
//     rc = Terminate;
//   else if (user_input == KEY_LEFT)
//     rc = Left;
//   else if (user_input == KEY_RIGHT)
//     rc = Right;
//   else if (user_input == KEY_UP)
//     rc = Up;
//   else if (user_input == KEY_DOWN)
//     rc = Down;
//   else if (user_input == ' ')
//     rc = Action;
//   return rc;
// }

void SumTetris(GameInfo_t *game_info, Tetramino_t tetramino) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetramino.figure[x][y] == 1) {
        game_info->field[tetramino.point->x + x]
                        [tetramino.point->y + y] = 1;
      }

    }
  }
}

// исправить ситуацию с state в теории его могло бы и не быть просто switch сделать?

void GameLoopTetris(ViewInterface *view) {
  bool no_break = TRUE;
  Tetris tetris;
  GameInfo_t game_info;
  Tetris_t tetris_info;
  Tetramino_t tetramino;
  State_t state;
  int signal = 0;
  view->DisplayOverlay();
  InputHandler input_handler;
  while (no_break) {
    tetris_info = tetris.GetTetrisInfo();
    state = tetris_info.state;
    int hold = 0;
    if (state == MOVING || state == START) signal = GET_USER_INPUT;
    if (signal == KEY_DOWN)  hold = 1;
    if (tetris_info.state == START && signal == ENTER_KEY) view->DisplayField();
    if(game_info.pause != 1) tetris.userInput(input_handler.GetSignal(signal), hold);
    game_info = tetris.updateCurrentState();
    tetris_info = tetris.GetTetrisInfo();
    state = tetris_info.state;
    if(game_info.pause == 1) {
        view->DisplayPause();
        int user_input = GET_USER_INPUT;
        if (user_input == P_KEY) {
            view->ClearPause();
            tetris.GameResume();
        }
    }
    if (state != START) {
      tetramino = tetris.GetTetramino();
      SumTetris(&game_info, tetramino);
      view->UpdateView(game_info);
      view->DisplayNextTetramino(game_info);
    }
    if (state == GAMEOVER) {
        view->DisplayGameOver(game_info);
        no_break = false;
    }
    if (state == EXIT_STATE) {
        no_break = false;
    }
  }
}

#endif // CONTROLLER_TETRIS_H