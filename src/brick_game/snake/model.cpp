#include "model.hpp"

int Snake::IsBodySnake(Position_t point) {
  int result = 1;
  for (Position_t pos : game_info_->snake->points) {
    if (pos.x == point.x || pos.y == point.y) result = 0;
  }
  return result;
}

void Snake::GetRandomCoordiantes() {
  Position_t point;
  point.x = rand() % 20;
  point.y = rand() % 10;

  while (!IsBodySnake(point)) {
    point.x = rand() % 20;
    point.y = rand() % 10;
  }
  game_info_->apple->x = point.x;
  game_info_->apple->y = point.y;
}

void Snake::StartGame() {
  clock_gettime(CLOCK_REALTIME, game_info_->time);
  // GetRealTime();
  game_info_->state = SPAWN;
}

int Snake::IsCollision(Position_t* new_point) {
  int result = 0;
  Snake_t *snake = game_info_->snake;
  Position_t point = snake->points.front();
  if (snake->direction == UP_DIRECTION) {
    point.x-=1;
  } else if (snake->direction == RIGHT_DIRECTION) {
    point.y+=1;
  } else if (snake->direction == DOWN_DIRECTION) {
    point.x+=1;
  } else if (snake->direction == LEFT_DIRECTION) {
    point.y-=1;
  }
  if (point.x < 0 || point.x > 19 || point.y < 0 || point.y > 9) {
    result = 1;
  } else {
    if (IsBodySnake(point)) result = 1;
  }
  if (result != 1) {
    new_point->x = point.x;
    new_point->y = point.y;
  }
  return result;
}

int Snake::IsEating(Position_t new_point) {
  int result = 0;
  Snake_t *snake = game_info_->snake;
  if (game_info_->apple->x == new_point.x || game_info_->apple->y == new_point.y) result = 1;
  return result;
}

void Snake::InitBoard(int field[ROWS_MAP][COLS_MAP]) {
  for (int x = 0; x < ROWS_MAP; x++) {
    for (int y = 0; y < COLS_MAP; y++) {
      field[x][y] = 0;
    }
  }
}

void Snake::SnakePosInit() {
  std::deque<Position_t> mydeq(4, {SNAKE_START_X, SNAKE_START_Y});
  for (Position_t pos : mydeq) {
    pos.y += 1;
  }
  game_info_->snake->points = mydeq;
  // game_info_->snake->points[0].x = SNAKE_START_X;
  // game_info_->snake->points[0].y = SNAKE_START_Y;
}

void Snake::StatsInit() {
  int N = 256;
  char buffer[N];
  FILE *fp = fopen(SCORE_FILE, "r");
  if (fp != NULL) {
    fgets(buffer, N, fp);
    char *score = strchr(buffer, ':');
    game_info_->high_score = atoi(++score);
    fclose(fp);
  } else {
    FILE *file = fopen(SCORE_FILE, "w");
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

void Snake::GetRealTime() {
  int hours = ((long int)game_info_->time->tv_sec / 3600) % 24 + 3;
  int minutes = (long int)game_info_->time->tv_sec % 3600 / 60;
  game_info_->realtime[0] = hours;
  game_info_->realtime[1] = minutes;
}

void Snake::Shifting() {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  // difficult level
  int delay = BASE_DELAY * pow(0.8, game_info_->level - 1);
  if (Offset(*game_info_->time, current_time) >= delay) {
    MoveDown();
    game_info_->time->tv_sec = current_time.tv_sec;
    game_info_->time->tv_nsec = current_time.tv_nsec;
  }

  if (game_info_->state != SPAWN) {
    game_info_->state = MOVING;
  }
}

void Snake::NewStatsSaveInit() {
  if (game_info_->score > game_info_->high_score) {
    game_info_->high_score = game_info_->score;
    FILE *file = fopen(SCORE_FILE, "w");
    fprintf(file, "score:%d", game_info_->score);
    fclose(file);
  }
}

void Snake::Check() {
  int count = 0;
  if (count == 1) game_info_->score += 100;
  if (count == 2) game_info_->score += 300;
  if (count == 3) game_info_->score += 700;
  if (count == 4) game_info_->score += 1500;
  if (game_info_->level < MAX_LVL)
    game_info_->level = game_info_->score / SCORE_FOR_NXT_LVL + 1;
  NewStatsSaveInit();
  if (game_info_->state != SPAWN) {
    game_info_->state = SHIFTING;
  }
}

void Snake::Spawn() {
  GetRandomCoordiantes();
}

void Snake::MoveUp() {
  if (game_info_->snake->points.front().x - 1 != game_info_->snake->points[1].x && game_info_->snake->points.front().y != game_info_->snake->points[1].y) game_info_->snake->direction = UP_DIRECTION;
}
void Snake::MoveDown() {
  if (game_info_->snake->points.front().x + 1 != game_info_->snake->points[1].x && game_info_->snake->points.front().y != game_info_->snake->points[1].y)game_info_->snake->direction = DOWN_DIRECTION;
}
void Snake::MoveRight() {
  if (game_info_->snake->points.front().x != game_info_->snake->points[1].x && game_info_->snake->points.front().y + 1 != game_info_->snake->points[1].y)game_info_->snake->direction = RIGHT_DIRECTION;
}
void Snake::MoveLeft() {
  if (game_info_->snake->points.front().x != game_info_->snake->points[1].x && game_info_->snake->points.front().y - 1 != game_info_->snake->points[1].y)game_info_->snake->direction = RIGHT_DIRECTION;
}

void Snake::MoveForward() {
  Position_t new_point;
  if (IsCollision(&new_point)) {
    game_info_->state = GAMEOVER;
  } else {
    if (!IsEating(new_point)) 
      game_info_->snake->points.pop_back();
    game_info_->snake->points.push_front(new_point);
  }
}

void Snake::GamePause() {
  game_info_->pause = 1;
}

void Snake::GameResume() {
  game_info_->pause = 0;
  game_info_->state = MOVING;
}

void Snake::GameOver() {
  game_info_->state = EXIT_STATE;
}

void Snake::ExitState() { game_info_->state = EXIT_STATE; }