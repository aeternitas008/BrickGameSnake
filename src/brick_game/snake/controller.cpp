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
  SnakeInfo_t snake_info = snake.GetSnakeInfo();

  action fsm_simple[6] = { nullptr, &Snake::Spawn, nullptr, &Snake::Shifting, &Snake::GameOver, &Snake::ExitState};
  action fsm_table[2][9] = {
      {&Snake::StartGame, nullptr, &Snake::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {&Snake::Check, &Snake::GamePause, &Snake::ExitState, &Snake::MoveLeft, &Snake::MoveRight, &Snake::MoveUp, &Snake::MoveDown, nullptr, &Snake::Check},
  };
  action act;
  if (snake_info.state != MOVING && snake_info.state != START) {
    act = fsm_simple[snake_info.state];
  } else  {
    int state = START;
    if (snake_info.state == START && signal == Start) PrintField();
    if (snake_info.state == MOVING) state = 1;
    act = fsm_table[state][signal];
  }
  if (act) {
    (snake.*act)();
  }
}

void SumSnake(GameInfo_t *game_info, SnakeInfo_t snake_info) {
  int x, y;
  for (int i = 0; i < snake_info.snake->points.size();i++) {
    x = snake_info.snake->points[i].x;
    y = snake_info.snake->points[i].y;
    game_info->field[x][y] = 1;
  }
  x = snake_info.apple->x;
  y = snake_info.apple->y;
  // для отображения другим цветом
  game_info->field[x][y] = 2;
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
  Snake snake;
  GameInfo_t game_info;
  SnakeInfo_t snake_info;
  int signal = 0, hold = 0;
  while (no_break) {
    game_info = snake.updateCurrentState();
    snake_info = snake.GetSnakeInfo();
    if (snake_info.state == MOVING || snake_info.state == START) signal = GET_USER_INPUT;
 
    userInput(GetSignal(signal), hold);
    game_info = snake.updateCurrentState();
    if(game_info.pause == 1) {
      PauseGame(snake, game_info);
    }
    if (snake_info.state != START) {
      SumSnake(&game_info, snake_info);
      UpdateView(game_info);
    }
    if (snake_info.state == GAMEOVER) PrintGameOver(game_info);
    // либо засунуть в такие моменты в параметр паузы 2, 3 геймовер и победа либо... просто добавить отдельную функцию и вызывать также и в qt
    // if (snake_info.pause == 1) PrintWin(game_info);
    if (snake_info.state == EXIT_STATE) no_break = FALSE;
  }
}