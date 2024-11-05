#include "model.hpp"

// Функция для проверки уникальности тетрамино в последних 4
bool Tetris::isUnique(int tetramino, const std::array<int, 4>& last_tetraminos) {
  for (const auto& t : last_tetraminos) {
      if (tetramino == t) {
          return false;  // Найдено совпадение
      }
  }
  return true;  // Совпадений нет
}

// Функция для добавления нового тетрамино в массив
void Tetris::AddTypeTetramino(int tetramino) {
std::array<int, 4>& last_tetraminos = tetris_->tetramino->last_tetramino;
  for (int i = 1; i < last_tetraminos.size(); i++) {
      last_tetraminos[i - 1] = last_tetraminos[i];
  }
  last_tetraminos.back() = tetramino;
}

void Tetris::GetPseudoRandomTypeTetramino() {
  srand(time(0));
  int tetramino = rand() % 7;
  while (!isUnique(tetramino, tetris_->tetramino->last_tetramino)) {
    tetramino = rand() % 7;
  }
  AddTypeTetramino(tetramino);
  tetris_->tetramino->next_type = tetramino;
}


void Tetris::StartGame() {
  tetris_->state = SPAWN;
}

void Tetris::GetFigure(Tetramino_t *tetramino) {
  int key = 0;
  if (tetramino->type >= 1 && tetramino->type <= 3) {
    key = (tetramino->type - 1) * 2 + (tetramino->variant % 2) + 1;
  } else if (tetramino->type >= 4 && tetramino->type <= 6) {
    key = tetramino->variant + (tetramino->type == 4 ? 7 : (tetramino->type == 5 ? 11 : 15));
  }
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      tetramino->figure[x][y] = TETRAMINO_FIGURES[key][x][y];
    }
  }
}

int Tetris::CheckNewVariant() {
  Tetramino_t tetramino_test = *tetris_->tetramino;
 tetramino_test.variant = (tetramino_test.variant + 1) % 4;
  GetFigure(&tetramino_test);
  return CheckTetramino(tetramino_test);
}

int Tetris::CheckTetramino(Tetramino_t tetramino) {
  int result = 0;
  for (int x = 0; x < 4 && result == 0; x++) {
    for (int y = 0; y < 4 && result == 0; y++) {
      if (tetramino.figure[x][y] == 1 &&
          (tetramino.point->y + y > 9 || tetramino.point->y + y < 0 ||
           tetramino.point->x + x < 0 || tetramino.point->x + x > 19 ||
           game_info_->field[tetramino.point->x + x][tetramino.point->y + y] ==
               1)) {
        result = 1;
      }
    }
  }
  return result;
}

void Tetris::InitBoard(int field[ROWS_MAP][COLS_MAP]) {
  for (int x = 0; x < ROWS_MAP; x++) {
    for (int y = 0; y < COLS_MAP; y++) {
      field[x][y] = 0;
    }
  }
}

void Tetris::AddTetraminoOnBoard() {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetris_->tetramino->figure[x][y] == 1) {
        game_info_->field[tetris_->tetramino->point->x + x]
                        [tetris_->tetramino->point->y + y] = 1;
      }
    }
  }
}

void Tetris::TetraminoPosInit() {
  tetris_->tetramino->point->x = TETR_START_X;
  tetris_->tetramino->point->y = TETR_START_Y;
}

void Tetris::StatsInit() {
  int N = 256;
  char buffer[N];
  FILE *fp = fopen(SCORE_FILE_TTR, "r");
  if (fp != NULL) {
    fgets(buffer, N, fp);
    char *score = strchr(buffer, ':');
    game_info_->high_score = atoi(++score);
    fclose(fp);
  } else {
    FILE *file = fopen(SCORE_FILE_TTR, "w");
    fprintf(file, "score:0");
    fclose(file);
    game_info_->high_score = 0;
  }
  game_info_->level = 1;
  game_info_->speed = 1;
  game_info_->score = 0;
}


int Tetris::Offset(struct timespec last_time, struct timespec current_time) {
  int seconds = (current_time.tv_sec - last_time.tv_sec) * 1000;
  int nanoseconds = (current_time.tv_nsec - last_time.tv_nsec) / 1e6;
  int total_offset = seconds + nanoseconds;
  return total_offset;
}

void Tetris::Shifting() {
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);
  mvprintw(34, 35, "%d", tetris_->tetramino->type);
  // difficult level
  int delay = BASE_DELAY * pow(0.8, game_info_->level - 1);
  if (Offset(*tetris_->time, current_time) >= delay) {
    mvprintw(34, 35, "%d", tetris_->tetramino->type);
    MoveDown();
    tetris_->time->tv_sec = current_time.tv_sec;
    tetris_->time->tv_nsec = current_time.tv_nsec;
  }
  napms(20);
  if (tetris_->state != SPAWN) {
    tetris_->state = MOVING;
  }
}

int Tetris::HasFullLine(int *line) {
  int Checker = 0;
  *line = 0;
  for (int x = 0; x < 20 && Checker < 10; x++) {
    Checker = 0;
    for (int y = 0; y < 10; y++) {
      if (game_info_->field[x][y] == 1) {
        Checker += 1;
      }
    }
    if (Checker == 10) *line = x;
  }
  return *line;
}

void Tetris::CheckLines(int *count) {
  int line = 0;
  while (HasFullLine(&line)) {
    (*count)++;
    for (int k = line; k > 0; k--) {
      for (int y = 0; y < 10; y++) {
        game_info_->field[k][y] = game_info_->field[k - 1][y];
      }
    }
  }
}

void Tetris::NewStatsSaveInit() {
  if (game_info_->score > game_info_->high_score) {
    game_info_->high_score = game_info_->score;
    FILE *file = fopen(SCORE_FILE_TTR, "w");
    fprintf(file, "score:%d", game_info_->score);
    fclose(file);
  }
}

void Tetris::Check() {
  int count = 0;
  CheckLines(&count);
  if (count == 1) game_info_->score += 100;
  if (count == 2) game_info_->score += 300;
  if (count == 3) game_info_->score += 700;
  if (count == 4) game_info_->score += 1500;
  if (game_info_->level < MAX_LVL)
    game_info_->level = game_info_->score / SCORE_FOR_NXT_LVL_TTR + 1;
    game_info_->speed = game_info_->level;
  NewStatsSaveInit();
  // delete?
  if (tetris_->state != SPAWN) {
    tetris_->state = SHIFTING;
  }
}

bool Tetris::CheckFirstLine() {
  bool result = false;
  for (int y = 0; y <= 10 && result != true; y++) {
    if (game_info_->field[0][y] == 1) {
      result = true;
    }
  }
  return result;
}

void Tetris::Spawn() {
  tetris_->tetramino->variant = 0;
  tetris_->tetramino->type = tetris_->tetramino->next_type;
  clock_gettime(CLOCK_REALTIME, tetris_->time);
  TetraminoPosInit();
  GetFigure(tetris_->tetramino);
  GetPseudoRandomTypeTetramino();
  Tetramino_t next_tetramino = *tetris_->tetramino;
  next_tetramino.type = tetris_->tetramino->next_type;
  GetFigure(&next_tetramino);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game_info_->next[i][j] = next_tetramino.figure[i][j];
    }
  }
  if (CheckTetramino(*tetris_->tetramino) || CheckFirstLine()) {
    mvprintw(35,36, "yes");
    tetris_->state = GAMEOVER;
  } else {
    tetris_->state = SHIFTING;
  }
}

void Tetris::MoveUp() {
  tetris_->hold = tetris_->hold;
  // stub
}

bool Tetris::IsNotBlockBelow() {
  bool result = 1;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetris_->tetramino->figure[x][y] == 1 &&
          (tetris_->tetramino->point->x + x >= 19)) {
        result = 0;
      } else if (tetris_->tetramino->figure[x][y] == 1 &&
                 game_info_->field[tetris_->tetramino->point->x + x + 1]
                                 [tetris_->tetramino->point->y + y] == 1)
        result = 0;
    }
  }
  return result;
}

bool Tetris::IsBlockRight() {
  bool result = 1;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetris_->tetramino->figure[x][y] == 1 &&
          (tetris_->tetramino->point->y + y >= 9 ||
           game_info_->field[tetris_->tetramino->point->x + x]
                           [tetris_->tetramino->point->y + y + 1] == 1)) {
        result = 0;
      }
    }
  }
  return result;
}

bool Tetris::IsBlockLeft() {
  bool result = 1;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (tetris_->tetramino->figure[x][y] == 1 &&
          (tetris_->tetramino->point->y + y <= 0 ||
           game_info_->field[tetris_->tetramino->point->x + x]
                           [tetris_->tetramino->point->y + y - 1] == 1)) {
        result = 0;
      }
    }
  }
  return result;
}

void Tetris::MoveDown() {
  if (IsNotBlockBelow()) {
    do {
      tetris_->tetramino->point->x += 1;
    } while (tetris_->hold && IsNotBlockBelow());
    if (tetris_->hold) {
      AddTetraminoOnBoard();
      tetris_->state = SPAWN;
      tetris_->hold = 0;
    }
  } else {
    AddTetraminoOnBoard();
    tetris_->state = SPAWN;
  }
  Check();
}

void Tetris::MoveRight() {
  if (IsBlockRight()) {
    tetris_->tetramino->point->y += 1;
  }
  tetris_->state = SHIFTING;
}

void Tetris::MoveLeft() {
  if (IsBlockLeft()) {
    tetris_->tetramino->point->y -= 1;
  }
  tetris_->state = SHIFTING;
}

void Tetris::TurnRight() {
  if (CheckNewVariant() == 0) {
    tetris_->tetramino->variant = (tetris_->tetramino->variant + 1) % 4;
    GetFigure(tetris_->tetramino);
  }
  tetris_->state = SHIFTING;
}

void Tetris::GamePause() {
  game_info_->pause = 1;
}

void Tetris::GameResume() {
  game_info_->pause = 0;
  tetris_->state = MOVING;
}

void Tetris::GameOver() {
  tetris_->state = EXIT_STATE;
}

void Tetris::ExitState() { tetris_->state = EXIT_STATE; }