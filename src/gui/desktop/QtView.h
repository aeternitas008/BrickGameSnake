// QtView.h
#ifndef QTVIEW_H
#define QTVIEW_H

#include <QObject>
#include "../ViewInterface.h"

class QtView : public QObject, public ViewInterface {
    Q_OBJECT
public:
    QtView(QObject *parent = nullptr) : QObject(parent) {}

    void UpdateView(const GameInfo_t game_info) override {
        emit UpdateGameInfo(game_info);
    }

    void DisplayPause() override {
        emit ShowPause();
    }

    void ClearPause() override {
        emit HidePause();
    }

    void DisplayGameOver(const GameInfo_t &game_info) override {
        emit ShowGameOver(game_info);
    }

    void DisplayGameWin() override {
        emit ShowGameWin();
    }

    void DisplayOverlay() override {
        emit PrintOverlay();
    }

        // Реализация метода DisplayNextTetramino
    void DisplayNextTetramino(GameInfo_t game_info) {
        // Ваша логика для отображения следующего тетрамино в графическом интерфейсе
        emit PrintNextTetramino(game_info);
    }

    
    // Реализация метода DisplayField
    void DisplayField() {
    // Ваша логика для отображения игрового поля в графическом интерфейсе
        emit PrintField();
    }


signals:
    void UpdateGameInfo(GameInfo_t game_info);
    void ShowPause();
    void HidePause();
    void ShowGameOver(GameInfo_t game_info);
    void ShowGameWin();
    void PrintOverlay();

    void PrintField();
    void PrintNextTetramino(GameInfo_t game_info);
};

#endif // QTVIEW_H
