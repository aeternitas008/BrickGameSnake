#include <QWidget>
#include <QKeyEvent>
#include <QObject>
#include "ModelTetris.h" // Модель Тетриса

class TetrisController : public QWidget {
    Q_OBJECT

public:
    explicit TetrisController(Tetris* tetrisModel, QWidget* parent = nullptr)
        : QWidget(parent), tetris(tetrisModel) {
        setFocusPolicy(Qt::StrongFocus); // Для получения событий клавиатуры
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {
        UserAction_t signal = GetSignal(event->key());
        bool hold = (event->key() == Qt::Key_Down); // Удержание клавиши вниз
        userInput(signal, hold);
    }

private:
    Tetris* tetris;

    UserAction_t GetSignal(int key) {
        switch (key) {
        case Qt::Key_Return:
            return Start;
        case Qt::Key_P:
            return Pause;
        case Qt::Key_Escape:
            return Terminate;
        case Qt::Key_Left:
            return Left;
        case Qt::Key_Right:
            return Right;
        case Qt::Key_Up:
            return Up;
        case Qt::Key_Down:
            return Down;
        case Qt::Key_Space:
            return Action;
        default:
            return Nosig;
        }
    }

    void userInput(UserAction_t signal, bool hold) {
        Tetris_t tetris_info = tetris->GetTetrisInfo();
        tetris->SetHold(hold);
        typedef void (Tetris::*action)();
        action fsm_simple[6] = { nullptr, &Tetris::Spawn, nullptr, &Tetris::Shifting, &Tetris::GameOver, &Tetris::ExitState };
        action fsm_table[2][9] = {
            { &Tetris::StartGame, nullptr, &Tetris::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            { nullptr, &Tetris::GamePause, &Tetris::ExitState, &Tetris::MoveLeft, &Tetris::MoveRight, &Tetris::MoveUp, &Tetris::MoveDown, &Tetris::TurnRight, &Tetris::Check }
        };
        
        action act;
        if (tetris_info.state != MOVING && tetris_info.state != START) {
            act = fsm_simple[tetris_info.state];
        } else {
            int state = (tetris_info.state == MOVING) ? 1 : 0;
            act = fsm_table[state][signal];
        }
        
        if (act) {
            (tetris->*act)();
        }
    }
};
