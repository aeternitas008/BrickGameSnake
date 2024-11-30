#ifndef QTVIEW_H
#define QTVIEW_H

#include <QObject>
#include "../ViewInterface.h"

class QtView : public QObject, public ViewInterface {
    Q_OBJECT
public:
    QtView(QObject *parent = nullptr) : QObject(parent) {}

    // Обновление информации о текущем состоянии игры
    void UpdateView(const GameInfo_t game_info) override {
        emit UpdateGameInfo(game_info);
    }

    // Отображение паузы
    void DisplayPause() override {
        emit ShowPause();
    }

    // Удаление паузы
    void ClearPause() override {
        emit HidePause();
    }

    // Отображение окончания игры
    void DisplayGameOver(const GameInfo_t &game_info) override {
        emit ShowGameOver(game_info);
    }

    // Отображение победы
    void DisplayGameWin() override {
        emit ShowGameWin();
    }

    // Отображение оверлея
    void DisplayOverlay() override {
        emit PrintOverlay();
    }

    // Реализация метода отображения следующего тетрамино (для тетриса)
    void DisplayNextTetramino(GameInfo_t game_info) {
        emit PrintNextTetramino(game_info);
    }

    // Отображение игрового поля
    void DisplayField() {
        emit PrintField();
    }

    // Обработка ввода с клавиатуры
    void handleKeyPress(int key) {
        emit KeyPressed(key);
    }

signals:
    // Сигналы для обновления информации о текущем состоянии игры
    void UpdateGameInfo(GameInfo_t game_info);

    // Сигналы для отображения разных экранов
    void ShowPause();
    void HidePause();
    void ShowGameOver(GameInfo_t game_info);
    void ShowGameWin();
    void PrintOverlay();

    // Сигналы для отображения поля и элементов игры
    void PrintField();
    void PrintNextTetramino(GameInfo_t game_info);

    // Сигнал для обработки ввода с клавиатуры
    void KeyPressed(int key);  // Обработка нажатия клавиш
};

#endif // QTVIEW_H
