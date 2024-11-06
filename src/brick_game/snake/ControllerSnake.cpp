#include "ControllerSnake.h"

UserAction_t GetSignal1(int user_input) {
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

void userInput1(UserAction_t signal, bool hold) {
  Snake snake;
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

void GameLoopSnake(ViewInterface *view) {
    bool no_break = true;
    Snake snake;
    GameInfo_t game_info;
    SnakeInfo_t snake_info;
    int signal = 0, hold = 0;
    view->DisplayOverlay();
    while (no_break) {
        game_info = snake.updateCurrentState();
        snake_info = snake.GetSnakeInfo();

        if (snake_info.state == MOVING || snake_info.state == START) 
            signal = GET_USER_INPUT;

        if (snake_info.state == START && signal == ENTER_KEY) view->DisplayField();
        if(game_info.pause != 1) userInput1(GetSignal1(signal), hold);
        game_info = snake.updateCurrentState();

        if(game_info.pause == 1) {
            view->DisplayPause();
            int user_input = GET_USER_INPUT;
            if (user_input == P_KEY) {
                view->ClearPause();
                snake.GameResume();
            }
        }
        if (snake_info.state != START) {
            SumSnake(&game_info, snake_info);
            view->UpdateView(game_info);
        }
        if (game_info.score == 200) {
            view->DisplayGameWin();
            no_break = false;
        }
        if (snake_info.state == GAMEOVER) {
            view->DisplayGameOver(game_info);
            no_break = false;
        }
        if (snake_info.state == EXIT_STATE) {
            no_break = false;
        }
    }
}