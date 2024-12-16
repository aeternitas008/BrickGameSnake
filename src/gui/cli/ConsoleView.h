// ConsoleView.h
#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <locale.h>
#include <unistd.h>
#include <wchar.h>
#include <ncurses.h>
#undef border
#include <cstdio>

#include "../../brick_game/defines.h"

class ConsoleView {
public:
    void PrintBoard(int[ROWS_MAP][COLS_MAP]);
    void PrintStats(GameInfo_t);
    void PrintPause();
    void ClearPause();
    void PrintGameOver(GameInfo_t);
    void PrintGameWin();
    void PrintOverlay();
    void PrintNextTetramino(int figure[4][4]);
    void PrintField();

    void UpdateView(GameInfo_t game_info) {
        PrintBoard(game_info.field);
        PrintStats(game_info);
    }
};

#endif // CONSOLEVIEW_H