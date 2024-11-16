// ConsoleView.h
#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <locale.h>
#include <unistd.h>
#include <wchar.h>
#include <ncurses.h>
#undef border
#include <cstdio>

#include "../../brick_game/defines.hpp"
#include "../ViewInterface.h"

class ConsoleView : public ViewInterface {
private:
    void PrintBoard(int[ROWS_MAP][COLS_MAP]);
    void PrintStats(GameInfo_t);
    void PrintPause();
    void HidePause();
    void PrintGameOver(GameInfo_t);
    void PrintGameWin();
    void PrintOverlay();
    void PrintNextTetramino(int figure[4][4]);

    void PrintField();
public:
    void UpdateView(GameInfo_t game_info) override {
        PrintBoard(game_info.field);
        PrintStats(game_info);
    }

    void DisplayPause() override {
        PrintPause();
    }

    void ClearPause() override {
        HidePause();
    }

    void DisplayGameOver(const GameInfo_t &gameInfo) override {
        PrintGameOver(gameInfo);
    }

    void DisplayGameWin() override {
        PrintGameWin();
    }

    void DisplayOverlay() override {
        PrintOverlay();
    }

    void DisplayNextTetramino(GameInfo_t gameInfo) override {
        PrintNextTetramino(gameInfo.next);
    }


    void DisplayField() override {
        PrintField();
    }
};

#endif // CONSOLEVIEW_H