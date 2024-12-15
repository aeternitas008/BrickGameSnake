// ViewInterface.h
#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H

#include "../brick_game/defines.h"  // Структуры данных вашей игры

class ViewInterface {
public:
    virtual ~ViewInterface() = default;

    virtual void UpdateView(const GameInfo_t gameInfo) = 0;
    virtual void DisplayPause() = 0;
    virtual void ClearPause() = 0;
    virtual void DisplayGameOver(const GameInfo_t &gameInfo) = 0;
    virtual void DisplayGameWin() = 0;
    virtual void DisplayOverlay() = 0;
    virtual void DisplayNextTetramino(GameInfo_t gameInfo) = 0;

    virtual void DisplayField() = 0;
};

#endif // VIEWINTERFACE_H
