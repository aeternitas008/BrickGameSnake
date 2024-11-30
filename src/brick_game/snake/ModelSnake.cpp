#include "ModelSnake.h"
#include "../defines.h"

void Snake::SumSnake(GameInfo_t *game_info, SnakeInfo_t snake_info) {
    int x, y;
    for (int i = 0; i < snake_info.snake->points.size(); i++) {
        x = snake_info.snake->points[i].x;
        y = snake_info.snake->points[i].y;
        game_info->field[x][y] = 1;
    }
    x = snake_info.apple->x;
    y = snake_info.apple->y;
    game_info->field[x][y] = 2;  // для отображения другим цветом
}

int Snake::IsBodySnake(Position_t point) {
  int result = 0;
  for (Position_t pos : snake_info_->snake->points) {
    if (pos.x == point.x && pos.y == point.y) result = 1;
  }
  return result;
}

void Snake::GetRandomCoordiantes() {
  Position_t point;
  point.x = rand() % 20;
  point.y = rand() % 10;

  while (IsBodySnake(point)) {
    point.x = rand() % 20;
    point.y = rand() % 10;
  }
  snake_info_->apple->x = point.x;
  snake_info_->apple->y = point.y;
}

void Snake::StartGame() {
  clock_gettime(CLOCK_REALTIME, snake_info_->time);
  snake_info_->state = SPAWN;
}

int Snake::IsCollision(Position_t point) {
  int result = 0;
  if (point.x < 0 || point.x > 19 || point.y < 0 || point.y > 9) {
    result = 1;
  } else {
    if (IsBodySnake(point)) result = 1;
  }
  return result;
}

int Snake::IsEating(Position_t new_point) {
  int result = 0;
  Snake_t *snake = snake_info_->snake;
  if (snake_info_->apple->x == new_point.x && snake_info_->apple->y == new_point.y) result = 1;
  return result;
}

void Snake::InitBoard() {
  for (int x = 0; x < ROWS_MAP; x++) {
    for (int y = 0; y < COLS_MAP; y++) {
      game_info_->field[x][y] = 0;
    }
  }
}

void Snake::SnakePosInit() {
  std::deque<Position_t> mydeq;
  Position_t pos = {SNAKE_START_X, SNAKE_START_Y};
  for (int i = 0; i < 4; ++i) {
    mydeq.push_back({pos.x, pos.y + i});
  }
  snake_info_->snake->points = mydeq;
}

void Snake::StatsInit() {
  int N = 256;
  char buffer[N];
  FILE *fp = fopen(SCORE_FILE_SNK, "r");
  if (fp != NULL) {
    fgets(buffer, N, fp);
    char *score = strchr(buffer, ':');
    game_info_->high_score = atoi(++score);
    fclose(fp);
  } else {
    FILE *file = fopen(SCORE_FILE_SNK, "w");
    fprintf(file, "score:0");
    fclose(file);
    game_info_->high_score = 0;
  }
  game_info_->level = 1;
  game_info_->speed = 1;
  game_info_->score = 0;
}


int Snake::Offset(struct timespec last_time, struct timespec current_time) {
  int seconds = (current_time.tv_sec - last_time.tv_sec) * 1000;
  int nanoseconds = (current_time.tv_nsec - last_time.tv_nsec) / 1e6;
  int total_offset = seconds + nanoseconds;
  return total_offset;
}

void Snake::Shifting() {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  // difficult level
  int delay = BASE_DELAY_SNK * pow(0.8, game_info_->level - 1);
  if (Offset(*snake_info_->time, current_time) >= delay) {
    MoveForward();
    snake_info_->time->tv_sec = current_time.tv_sec;
    snake_info_->time->tv_nsec = current_time.tv_nsec;
  }

  if (snake_info_->state != GAMEOVER && snake_info_->state != SPAWN) {
    snake_info_->state = MOVING;
  }
}

void Snake::NewStatsSaveInit() {
  if (game_info_->score > game_info_->high_score) {
    game_info_->high_score = game_info_->score;
    FILE *file = fopen(SCORE_FILE_SNK, "w");
    fprintf(file, "score:%d", game_info_->score);
    fclose(file);
  }
}

void Snake::Check() {
  if (game_info_->level < MAX_LVL)
    game_info_->level = game_info_->score / SCORE_FOR_NXT_LVL_SNK + 1;
    game_info_->speed = game_info_->level;
    NewStatsSaveInit();
    snake_info_->state = SHIFTING;
}

void Snake::Spawn() {
  GetRandomCoordiantes();
  snake_info_->state = SHIFTING;
}

void Snake::MoveUp() {
  if (snake_info_->snake->points.front().x - 1 != snake_info_->snake->points[1].x) snake_info_->snake->direction = UP_DIRECTION;
  snake_info_->state = SHIFTING;
}
void Snake::MoveDown() {
  if (snake_info_->snake->points.front().x + 1 != snake_info_->snake->points[1].x)snake_info_->snake->direction = DOWN_DIRECTION;
  snake_info_->state = SHIFTING;
}
void Snake::MoveRight() {
  if (snake_info_->snake->points.front().y + 1 != snake_info_->snake->points[1].y)snake_info_->snake->direction = RIGHT_DIRECTION;
  snake_info_->state = SHIFTING;
}
void Snake::MoveLeft() {
  if (snake_info_->snake->points.front().y - 1 != snake_info_->snake->points[1].y)snake_info_->snake->direction = LEFT_DIRECTION;
  snake_info_->state = SHIFTING;
}

void Snake::MoveForward() {
  Snake_t *snake = snake_info_->snake;
  Position_t new_point = snake->points.front();
  if (snake->direction == UP_DIRECTION) {
    new_point.x-=1;
  } else if (snake->direction == RIGHT_DIRECTION) {
    new_point.y+=1;
  } else if (snake->direction == DOWN_DIRECTION) {
    new_point.x+=1;
  } else if (snake->direction == LEFT_DIRECTION) {
    new_point.y-=1;
  }
  if (IsCollision(new_point)) {
    snake_info_->state = GAMEOVER;
  } else {
    if (!IsEating(new_point)) {
      snake_info_->snake->points.pop_back();
      snake_info_->state = SHIFTING;
    } else {
      game_info_->score++;
      Check();
      snake_info_->state = SPAWN;
    }
    snake_info_->snake->points.push_front(new_point);
  }
}

void Snake::userInput(UserAction_t signal, bool hold) {
  // Определяем тип указателя на метод класса Snake
  typedef void (Snake::*action)();
  // Машины состояний с указателями на методы класса Snake
  action fsm_simple[6] = { nullptr, &Snake::Spawn, nullptr, &Snake::Shifting, &Snake::GameOver, &Snake::ExitState };
  action fsm_table[2][9] = {
      { &Snake::StartGame, nullptr, &Snake::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
      { &Snake::Check, &Snake::GamePause, &Snake::ExitState, &Snake::MoveLeft, &Snake::MoveRight, &Snake::MoveUp, &Snake::MoveDown, nullptr, &Snake::Check },
  };
  // Определяем указатель на метод
  action act = nullptr;
  // Логика выбора действия в зависимости от текущего состояния змейки
  if (snake_info_->state != MOVING && snake_info_->state != START) {
    act = fsm_simple[snake_info_->state];
  } else {
    int state = (snake_info_->state == MOVING) ? 1 : 0;
    act = fsm_table[state][signal];
  }
  // Если действие определено, вызываем его для текущего объекта
  if (act) {
    (this->*act)();  // Вызов метода через указатель на метод для текущего объекта
  }
}
