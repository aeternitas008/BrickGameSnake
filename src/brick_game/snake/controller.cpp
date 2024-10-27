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

typedef void (Snake::*action)();

void userInput(UserAction_t signal, bool hold) {
  Snake snake;
  GameInfo_t game_info = snake.updateCurrentState();

  action fsm_simple[6] = { nullptr, &Snake::Spawn, nullptr, &Snake::Shifting, &Snake::GameOver, &Snake::ExitState};

  action fsm_table[2][9] = {
      {&Snake::StartGame, nullptr, &Snake::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {&Snake::Check, &Snake::GamePause, &Snake::ExitState, &Snake::MoveLeft, &Snake::MoveRight, &Snake::MoveUp, &Snake::MoveDown, nullptr, &Snake::Check},
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
    (snake.*act)();
  }
}

void PauseGame(Snake Snake, GameInfo_t game_info) {
      PrintPause();
      int user_input = -1;
      while (user_input != ESCAPE_KEY && user_input != P_KEY &&
            user_input != ENTER_KEY) {
        user_input = GET_USER_INPUT;
      }
      ClearPause();
      Snake.GameResume();
}

void GameLoop() {
  bool no_break = TRUE;
  Snake Snake;
  GameInfo_t game_info;
  int signal = 0, hold_down = 0;
  while (no_break) {
    game_info = Snake.updateCurrentState();
    Snake.GetRealTime();
    PrintTime(game_info.realtime);
    int hold = 0;
    if (game_info.state == MOVING || game_info.state == START) signal = GET_USER_INPUT;
 
    userInput(GetSignal(signal), hold);
    // if (game_info.state == SPAWN) {
    //   flushinp();
    //   hold_down = 0;  // Сбрасываем hold_down, чтобы новое тетрамино не двигалось сразу быстро вниз
    // }
    if(game_info.pause == 1) {
      PauseGame(Snake, game_info);
    }
    if (game_info.state != START) {
      UpdateView(game_info);
      PrintNextTetramino(game_info.next);
    }
    if (game_info.state == GAMEOVER) PrintGameOver(game_info);
    if (game_info.state == EXIT_STATE) no_break = FALSE;
    napms(10);
  }
}