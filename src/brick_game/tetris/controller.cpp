#include "controller.hpp"

UserAction_t GetSignal(int user_input) {
  UserAction_t rc = Nosig;
  if (user_input == ENTER_KEY)
    rc = Start;
  else if (user_input == P_KEY_SMALL || user_input == P_KEY)
    rc = Pause;
  else if (user_input == ESCAPE_KEY)
    rc = Terminate;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == KEY_UP)
    rc = Up;
  else if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == ' ')
    rc = Action;
  return rc;
}

typedef void (Tetris::*action)();

void userInput(UserAction_t signal, bool hold) {
  Tetris tetris;
  State_t state = tetris.GetState();
  GameInfo_t game_info = tetris.updateCurrentState();
  tetris.SetHold(hold);
  action fsm_simple[6] = { nullptr, &Tetris::Spawn, nullptr, nullptr, &Tetris::GameOver, &Tetris::ExitState};
  action fsm_table[2][9] = {
      {&Tetris::StartGame, nullptr, &Tetris::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {nullptr, &Tetris::GamePause, &Tetris::ExitState, &Tetris::MoveLeft, &Tetris::MoveRight, &Tetris::MoveUp, &Tetris::MoveDown, &Tetris::TurnRight, &Tetris::Check},
  };
  action act;
  if (state != MOVING && state != START) {
    act = fsm_simple[state];
  } else  {
    int i = state;
    if (state == START && signal == Start) PrintField();
    if (state == MOVING) i = 1;
    act = fsm_table[i][signal];
  }
  if (act) {
    (tetris.*act)();
  }
}

void PauseGame(Tetris tetris, GameInfo_t game_info) {
      PrintPause();
      int user_input = -1;
      while (user_input != ESCAPE_KEY && user_input != P_KEY &&
            user_input != ENTER_KEY) {
        user_input = GET_USER_INPUT;
      }
      ClearPause();
      tetris.GameResume();
}

void BoardPlusTetramino(GameInfo_t *game_info, Tetramino_t tetramino) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetramino.figure[x][y] == 1) {
        game_info->field[tetramino.point->x + x]
                        [tetramino.point->y + y] = 1;
      }
    }
  }
}

void GameLoop() {
  bool no_break = TRUE;
  Tetris tetris;
  GameInfo_t game_info;
  // Tetris_t tetris_info;
  Tetramino_t tetramino;
  State_t state;
  int signal = 0;
  while (no_break) {
    game_info = tetris.updateCurrentState();
    tetramino = tetris.GetTetramino();
    state = tetris.GetState();
    mvprintw(30,5, "%d", state);
    int hold = 0;
    if (state == MOVING || state == START) signal = GET_USER_INPUT;
    if (signal == KEY_DOWN) {
      hold = 1;
    }
    userInput(GetSignal(signal), hold);
    if(game_info.pause == 1) {
      PauseGame(tetris, game_info);
    }
    if (state != START) {
      tetris.Shifting();
      // BoardPlusTetramino(&game_info, tetramino);
      UpdateView(game_info);
      PrintNextTetramino(game_info.next);
    }
    if (state == GAMEOVER) PrintGameOver(game_info);
    if (state == EXIT_STATE) no_break = FALSE;
    napms(10);
  }
}