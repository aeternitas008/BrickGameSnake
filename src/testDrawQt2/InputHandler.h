// InputHandler.h
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../brick_game/defines.hpp"
// #ifdef timeout
// #undef timeout
// #endif
// #include <ncurses.h>
#include <QKeyEvent>


#define KEY_DOWN 0402
#define KEY_UP 0403
#define KEY_LEFT 0404
#define KEY_RIGHT 0405

// enum class UserAction_t {
//     Nosig, Start, Pause, Terminate, Left, Right, Up, Down, Action
// };

class InputHandler {
public:
    UserAction_t GetSignal(int key) {
        UserAction_t signal = Nosig;
        switch (key) {
        case Qt::Key_Return:
        case ENTER_KEY:
            signal = Start;
            break;
        case Qt::Key_P:
        case P_KEY:
            signal =  Pause;
            break;
        case Qt::Key_Escape:
        case ESCAPE_KEY:
            signal =  Terminate;
            break;
        case Qt::Key_Left:
        case KEY_LEFT:
            signal =  Left;
            break;
        case Qt::Key_Right:
        case KEY_RIGHT:
            signal =  Right;
            break;
        case Qt::Key_Up:
        case KEY_UP:
            signal =  Up;
            break;
        case Qt::Key_Down:
        case KEY_DOWN:
            signal =  Down;
            break;
        case Qt::Key_Space:
            signal =  Action;
            break;
        }
        return signal;
    }
};

#endif  // INPUT_HANDLER_H