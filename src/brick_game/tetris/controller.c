#include "controller.h"


void userInput(UserAction_t action, bool hold) {
  Params_t *s_prms = GetPrmsInstance();
  s_prms->sig = action;
  s_prms->hold = hold;
}

UserAction_t GetSignal(int user_input) {
  UserAction_t rc = NOSIG;
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

typedef void (*action)(Params_t *prms);

void SigAct(UserAction_t sig) {
  action fsm_table[7][9] = {
      {StartGame, NULL, ExitState, NULL, NULL, NULL, NULL, NULL, NULL},
      {Spawn, Spawn, Spawn, Spawn, Spawn, Spawn, Spawn, Spawn, Spawn},
      {Check, GamePause, ExitState, MoveLeft, MoveRight, MoveUp, MoveDown,
       TurnRight, Check},
      {Shifting, Shifting, ExitState, Shifting, Shifting, Shifting, Shifting,
       Shifting, Shifting},
      {GameOver, GameOver, GameOver, GameOver, GameOver, GameOver, GameOver,
       GameOver, GameOver},
      {ExitState, ExitState, ExitState, ExitState, ExitState, ExitState,
       ExitState, Check, Check}};
  action act = NULL;
  userInput(sig, sig == Down);
  Params_t *prms = GetPrmsInstance();
  act = fsm_table[*prms->state][sig];
  if (act) act(prms);
}

void GameLoop() {
  bool no_break = TRUE;
  int signal = 0;
  Params_t *prms = GetPrmsInstance();
  State_t state = START;
  GameInfo_t stats;
  Position tetramino_pos;
  struct timespec time;
  InitBoard(stats.field);
  TetraminoPosInit(&tetramino_pos);
  Tetramino_t tetramino = {0};
  tetramino.point = &tetramino_pos;
  tetramino.variant = 0;
  tetramino.type = 0;
  StatsInit(&stats);
  prms->stats = &stats;
  prms->state = &state;
  prms->tetramino = &tetramino;
  prms->time = &time;

  while (no_break) {
    if (state == MOVING || state == START) signal = GET_USER_INPUT;
    SigAct(GetSignal(signal));
    if (state == EXIT_STATE) no_break = FALSE;
    GameInfo_t stats = updateCurrentState();
    UpdateView(stats);
  }
}