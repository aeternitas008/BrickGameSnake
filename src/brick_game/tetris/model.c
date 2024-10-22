#include "model.h"

Params_t *GetPrmsInstance() {
  static Params_t s_prms;
  return &s_prms;
}

GameInfo_t updateCurrentState() {
  Params_t *prms = GetPrmsInstance();
  return *prms->stats;
}

void StartGame(Params_t *prms) {
  PrintField();
  TetraminoPosInit(prms->tetramino->point);
  clock_gettime(CLOCK_REALTIME, prms->time);
  StatsInit(prms->stats);
  *prms->state = SPAWN;
}


void GetTetramino(Tetramino_t *tetramino) {
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

int CheckNewVariant(Params_t prms) {
  Tetramino_t tetramino_test = *prms.tetramino;
  tetramino_test.variant = (tetramino_test.variant + 1) % 4;
  GetTetramino(&tetramino_test);
  return CheckTetramino(prms, tetramino_test);
}

int CheckTetramino(Params_t prms, Tetramino_t tetramino) {
  int result = 0;
  for (int x = 0; x < 4 && result == 0; x++) {
    for (int y = 0; y < 4 && result == 0; y++) {
      if (tetramino.figure[x][y] == 1 &&
          (tetramino.point->y + y > 9 || tetramino.point->y + y < 0 ||
           tetramino.point->x + x < 0 || tetramino.point->x + x > 19 ||
           prms.stats->field[tetramino.point->x + x][tetramino.point->y + y] ==
               1)) {
        result = 1;
      }
    }
  }
  return result;
}

void InitBoard(int field[ROWS_MAP][COLS_MAP]) {
  for (int x = 0; x < ROWS_MAP; x++) {
    for (int y = 0; y < COLS_MAP; y++) {
      field[x][y] = 0;
    }
  }
}

void AddTetraminoOnBoard(Params_t *prms) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (prms->tetramino->figure[x][y] == 1) {
        prms->stats->field[prms->tetramino->point->x + x]
                        [prms->tetramino->point->y + y] = 1;
      }
    }
  }
}


void TetraminoPosInit(Position *tetramino_pos) {
  tetramino_pos->x = TETRSTART_X;
  tetramino_pos->y = TETRSTART_Y;
}

void StatsInit(GameInfo_t *stats) {
  char *fname = SCORE_FILE;
  int N = 256;
  char buffer[N];
  FILE *fp = fopen(fname, "r");
  if (fp != NULL) {
    fgets(buffer, N, fp);
    char *score = strchr(buffer, ':');
    stats->high_score = atoi(++score);
    fclose(fp);
  } else {
    FILE *file = fopen(fname, "w");
    fprintf(file, "score:0");
    fclose(file);
    stats->high_score = 0;
  }
  stats->level = 1;
  stats->speed = 1;
  stats->score = 0;
  PrintStats(stats);
}


int Offset(struct timespec last_time, struct timespec current_time) {
  int seconds = (current_time.tv_sec - last_time.tv_sec) * 1000;
  int nanoseconds = (current_time.tv_nsec - last_time.tv_nsec) / 1e6;
  int total_offset = seconds + nanoseconds;
  return total_offset;
}

void Shifting(Params_t *prms) {
  int hours = ((long int)prms->time->tv_sec / 3600) % 24 + 3;
  int minutes = (long int)prms->time->tv_sec % 3600 / 60;
  PrintTime(hours, minutes);
  struct timespec current_time;
  clock_gettime(CLOCK_REALTIME, &current_time);

  // difficult level
  int delay = BASE_DELAY * pow(0.8, prms->stats->level - 1);
  if (Offset(*prms->time, current_time) >= delay) {
    MoveDown(prms);
    prms->time->tv_sec = current_time.tv_sec;
    prms->time->tv_nsec = current_time.tv_nsec;
  }

  if (*prms->state != SPAWN) {
    *prms->state = MOVING;
  }
}

int HasFullLine(int field[ROWS_MAP][COLS_MAP], int *line) {
  int Checker = 0;
  *line = 0;
  for (int x = 0; x < 20 && Checker < 10; x++) {
    Checker = 0;
    for (int y = 0; y < 10; y++) {
      if (field[x][y] == 1) {
        Checker += 1;
      }
    }
    if (Checker == 10) *line = x;
  }
  return *line;
}

void CheckLines(int *count, Params_t *prms) {
  int line = 0;
  while (HasFullLine(prms->stats->field, &line)) {
    (*count)++;
    for (int k = line; k > 0; k--) {
      for (int y = 0; y < 10; y++) {
        prms->stats->field[k][y] = prms->stats->field[k - 1][y];
      }
    }
  }
}

void NewStatsSaveInit(GameInfo_t *stats) {
  char *fname = SCORE_FILE;
  if (stats->score > stats->high_score) {
    stats->high_score = stats->score;
    FILE *file = fopen(fname, "w");
    fprintf(file, "score:%d", stats->score);
    fclose(file);
  }
}

void Check(Params_t *prms) {
  int count = 0;
  CheckLines(&count, prms);
  if (count == 1) prms->stats->score += 100;
  if (count == 2) prms->stats->score += 300;
  if (count == 3) prms->stats->score += 700;
  if (count == 4) prms->stats->score += 1500;
  if (prms->stats->level < MAX_LVL)
    prms->stats->level = prms->stats->score / SCORE_FOR_NXT_LVL + 1;
  NewStatsSaveInit(prms->stats);
  refresh();

  if (*prms->state != SPAWN) {
    *prms->state = SHIFTING;
  }
}

bool CheckFirstLine(Params_t *prms) {
  bool result = false;
  for (int y = 0; y <= 10 && result != true; y++) {
    if (prms->stats->field[0][y] == 1) {
      result = true;
    }
  }
  return result;
}

void Spawn(Params_t *prms) {
  prms->tetramino->variant = 0;
  TetraminoPosInit(prms->tetramino->point);
  GetTetramino(prms->tetramino);
  PrintBoard(prms->stats->field);
  if (CheckTetramino(*prms, *prms->tetramino) == 1 || CheckFirstLine(prms)) {
    *prms->state = GAMEOVER;
  } else {
    PrintTetramino(*prms->tetramino);
    PrintNextTetramino(prms);
    *prms->state = MOVING;
  }
}

void MoveUp(Params_t *prms) {
  prms->hold = prms->hold;
  // stub
}

bool IsNotBlockBelow(Params_t *prms) {
  bool result = 1;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (prms->tetramino->figure[x][y] == 1 &&
          (prms->tetramino->point->x + x >= 19)) {
        result = 0;
      } else if (prms->tetramino->figure[x][y] == 1 &&
                 prms->stats->field[prms->tetramino->point->x + x + 1]
                                 [prms->tetramino->point->y + y] == 1)
        result = 0;
    }
  }
  return result;
}

bool IsBlockRight(Params_t *prms) {
  bool result = 1;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (prms->tetramino->figure[x][y] == 1 &&
          (prms->tetramino->point->y + y >= 9 ||
           prms->stats->field[prms->tetramino->point->x + x]
                           [prms->tetramino->point->y + y + 1] == 1)) {
        result = 0;
      }
    }
  }
  return result;
}

bool IsBlockLeft(Params_t *prms) {
  bool result = 1;
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (prms->tetramino->figure[x][y] == 1 &&
          (prms->tetramino->point->y + y <= 0 ||
           prms->stats->field[prms->tetramino->point->x + x]
                           [prms->tetramino->point->y + y - 1] == 1)) {
        result = 0;
      }
    }
  }
  return result;
}

void MoveDown(Params_t *prms) {
  if (IsNotBlockBelow(prms)) {
    do {
      ClearTetramino(*prms->tetramino);
      prms->tetramino->point->x += 1;
      PrintTetramino(*prms->tetramino);
    } while (prms->hold && IsNotBlockBelow(prms));
    refresh();
    if (prms->hold) {
      AddTetraminoOnBoard(prms);
      prms->tetramino->type = (prms->tetramino->type + 1) % 7;
      *prms->state = SPAWN;
      prms->hold = 0;
    }
  } else {
    AddTetraminoOnBoard(prms);
    prms->tetramino->type = (prms->tetramino->type + 1) % 7;
    *prms->state = SPAWN;
  }
  Check(prms);
}

void MoveRight(Params_t *prms) {
  if (IsBlockRight(prms)) {
    ClearTetramino(*prms->tetramino);
    prms->tetramino->point->y += 1;
    PrintTetramino(*prms->tetramino);
    refresh();
  }
  Check(prms);
}

void MoveLeft(Params_t *prms) {
  if (IsBlockLeft(prms)) {
    ClearTetramino(*prms->tetramino);
    prms->tetramino->point->y -= 1;
    PrintTetramino(*prms->tetramino);
    refresh();
  }
  Check(prms);
}

void TurnRight(Params_t *prms) {
  if (CheckNewVariant(*prms) == 0) {
    prms->tetramino->variant = (prms->tetramino->variant + 1) % 4;
    ClearTetramino(*prms->tetramino);
    GetTetramino(prms->tetramino);
    PrintTetramino(*prms->tetramino);
  }
  *prms->state = MOVING;
}

void GamePause(Params_t *prms) {
  PrintPause();
  int user_input = 0;
  while (user_input != ESCAPE_KEY && user_input != P_KEY &&
         user_input != ENTER_KEY) {
    user_input = GET_USER_INPUT;
  }
  ClearPause();
  // @todo previos UpdateCurentState
  UserAction_t signal = KEY_CANCEL;
  if (user_input == ESCAPE_KEY) signal = Terminate;
  if (signal == Terminate) {
    *prms->state = EXIT_STATE;
  } else {
    *prms->state = MOVING;
  }
}

void GameOver(Params_t *prms) {
  PrintGameOver(prms->stats);
  *prms->state = EXIT_STATE;
}


void ExitState(Params_t *prms) { *prms->state = EXIT_STATE; }