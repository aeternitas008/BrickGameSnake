#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include "QtView.h"
#include "../../brick_game/defines.hpp"

const int BLOCK_SIZE = 400 / 10;  // 40 пикселей на блок

class GameWindow : public QWidget {
    Q_OBJECT
public:
    GameWindow(QWidget *parent = nullptr);

    // Подключение к сигналам QtView
    void setView(QtView *view);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;  // Обработка ввода с клавиатуры

private slots:
    void onUpdateGameInfo(GameInfo_t gameInfo);
    void onShowPause();
    void onHidePause();
    void onShowGameOver(GameInfo_t gameInfo);
    void onShowGameWin();

private:
    GameInfo_t currentGameInfo;
    bool isPaused;
};

#endif // GAMEWINDOW_H
