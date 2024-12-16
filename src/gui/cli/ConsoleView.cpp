#include "ConsoleView.h"

void PrintRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void ConsoleView::PrintOverlay() {
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  PrintRectangle(0, BOARD_N - 1, 0, BOARD_M + 1);
  PrintRectangle(0, BOARD_N - 1, BOARD_M + 2, BOARD_M + HUD_WIDTH + 5);

  for (int i = 1; i <= 4; i++) {
    int a = 0;
    if (i == 4) a = 3;
    PrintRectangle(i + (i - 1) * 3, i * 4 + a, BOARD_M + 3,
                   BOARD_M + HUD_WIDTH + 4);
  }

  MVPRINTW(2, BOARD_M + 5, "HIGH SCORE");
  MVPRINTW(3, BOARD_M + 8, "%05d", 0);

  MVPRINTW(6, BOARD_M + 8, "SCORE");
  MVPRINTW(7, BOARD_M + 8, "%05d", 0);

  MVPRINTW(10, BOARD_M + 7, "LVL SPD");
  MVPRINTW(11, BOARD_M + 8, "%02d  %02d", 1, 1);

  MVPRINTW(14, BOARD_M + 8, "NEXT");
  MVPRINTW(15, BOARD_M + 4, "[ ][ ][ ][ ]");
  MVPRINTW(16, BOARD_M + 4, "[ ][ ][ ][ ]");
  MVPRINTW(17, BOARD_M + 4, "[ ][ ][ ][ ]");
  MVPRINTW(18, BOARD_M + 4, "[ ][ ][ ][ ]");

  MVPRINTW(BOARD_N / 2, (BOARD_M - INTRO_MESSAGE_LEN) / 2 + 1, INTRO_MESSAGE);
}

void ConsoleView::PrintStats(GameInfo_t stats) {
  MVPRINTW(3, BOARD_M + 8, "%05d", stats.high_score);
  MVPRINTW(7, BOARD_M + 8, "%05d", stats.score);
  MVPRINTW(11, BOARD_M + 8, "%02d  %02d", stats.level, stats.speed);
}

void ConsoleView::PrintField() {
  for (int i = 3; i <= 22; i++) {
    MVPRINTW(i, 1, "[ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]");
    refresh();
    napms(30);
  }
}

void ConsoleView::PrintPause() {
  MVPRINTW(1, 2, "Press ENTER or P to continue");
  MVPRINTW(2, 8, "or ESC to exit");
}

/**
 * @brief Очищает сообщение о паузе с экрана.
 */
void ConsoleView::ClearPause() {
  MVPRINTW(1, 2, "                            ");
  MVPRINTW(2, 8, "              ");
}

void ConsoleView::PrintBoard(int field[ROWS_MAP][COLS_MAP]) {
  for (int x = 0; x < ROWS_MAP; x++) {
    for (int y = 0; y < COLS_MAP; y++) {
      if (field[x][y] == 1) {
        MVPRINTW(x + BOARDS_BEGIN + 1, y * 3 + 2, BLOCK);
      } else if (field[x][y] == 2) {
        attron(COLOR_PAIR(3));
        MVPRINTW(x + BOARDS_BEGIN + 1, y * 3 + 2, BLOCK);
        attron(COLOR_PAIR(2));
      } else {
        CLEAR_BACKPOS(x + BOARDS_BEGIN + 1, y * 3 + 2);
      }
    }
  }
}

void ConsoleView::PrintGameOver(GameInfo_t stats) {
  MVPRINTW(1, 10, "GAME OVER");
  MVPRINTW(2, 8, "%s %d", "Your score: ", stats.score);
  refresh();
  napms(3000);
}

void ConsoleView::PrintGameWin() {
  MVPRINTW(1, 10, "YOU WIN");
  MVPRINTW(2, 9, "%s", "Come again!");
  refresh();
  napms(3000);
}

void ConsoleView::PrintNextTetramino(int figure[4][4]) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (figure[x][y]) {
        MVPRINTW(15 + x, 33 + y * 3 + 2, BLOCK);
      } else {
        CLEAR_BACKPOS(15 + x, 33 + y * 3 + 2);
      }
    }
  }
}