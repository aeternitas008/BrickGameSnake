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
  GameInfo_t game_info = tetris.updateCurrentState();
  if (hold) tetris.SetHold(hold);
  action fsm_simple[6] = { nullptr, &Tetris::Spawn, nullptr, &Tetris::Shifting, &Tetris::GameOver, &Tetris::ExitState};

  action fsm_table[2][9] = {
      {&Tetris::StartGame, nullptr, &Tetris::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {&Tetris::Check, &Tetris::GamePause, &Tetris::ExitState, &Tetris::MoveLeft, &Tetris::MoveRight, &Tetris::MoveUp, &Tetris::MoveDown, &Tetris::TurnRight, &Tetris::Check},
  };
  action act;
  if (game_info.state != MOVING && game_info.state != START) {
    act = fsm_simple[game_info.state];
  } else  {
    int state = START;
    if (game_info.state == START && signal == Start) PrintField();
    if (game_info.state == MOVING) state = 1;
    act = fsm_table[state][signal];
  }
  if (act) {
    (tetris.*act)();
  }
}

void GameLoop() {
  bool no_break = TRUE;
  Tetris tetris;
  GameInfo_t game_info = tetris.updateCurrentState();
  tetris.GetRealTime();
  PrintTime(game_info.realtime);
  int signal = 0;
  struct timespec hold_start_time;
  int hold_down = 0;
  while (no_break) {
    game_info = tetris.updateCurrentState();
    int hold = 0;
    if (game_info.state == MOVING || game_info.state == START) signal = GET_USER_INPUT;
    if (signal == KEY_DOWN) {
      if (hold_down == 0) {
        clock_gettime(CLOCK_REALTIME, &hold_start_time);
        hold_down = 1;
      } else {
        struct timespec current_time;
        clock_gettime(CLOCK_REALTIME, &current_time);
        if (tetris.Offset(hold_start_time, current_time) >= 200) {
          MVPRINTW(29, 30, "%s", "elaps yes, hold 1");
          hold = 1;
          hold_down = 0;
        }
      }
    } 
    else hold_down = 0;
    userInput(GetSignal(signal), hold);

    if(game_info.pause == 1) {
      PrintPause();
      int user_input = -1;
      while (user_input != ESCAPE_KEY && user_input != P_KEY &&
            user_input != ENTER_KEY) {
        user_input = GET_USER_INPUT;
      }
      ClearPause();
      tetris.GameResume();
    }

    if (game_info.state != START) {
      UpdateView(game_info);
      PrintNextTetramino(game_info.next);
    }

    MVPRINTW(30, 30, "%s %d", "hold", hold); 
    if (game_info.state == GAMEOVER) PrintGameOver (game_info);
    if (game_info.state == EXIT_STATE) no_break = FALSE;
    napms(15);
  }
}