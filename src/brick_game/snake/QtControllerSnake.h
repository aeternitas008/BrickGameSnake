#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include "ModelSnake.h" // Модель игры Snake
#include "QtView.h"     // Представление игры

class SnakeController : public QWidget {
    Q_OBJECT

public:
    explicit SnakeController(Snake* snakeModel, QtView* view, QWidget* parent = nullptr)
        : QWidget(parent), snake(snakeModel), view(view) {
        setFocusPolicy(Qt::StrongFocus); // Для получения событий клавиатуры

        // Настроим таймер для игрового цикла
        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &SnakeController::updateGame);
        gameTimer->start(100); // Запуск таймера с интервалом 100 мс

        // Подписываемся на сигнал ввода с клавиатуры
        connect(view, &QtView::KeyPressed, this, &SnakeController::keyPressHandler);
    }

    void startGameLoop() {
        // Запуск игрового таймера
        gameTimer->start();
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {
        // Отправляем сигнал нажатия клавиши в QtView
        view->handleKeyPress(event->key());
    }

private:
    Snake* snake;
    QtView* view;
    QTimer* gameTimer;
    bool isRunning = true;

    // Обработчик нажатия клавиш
    void keyPressHandler(int key) {
        UserAction_t signal = GetSignal(key);
        bool hold = (key == Qt::Key_Down); // Удержание клавиши вниз
        userInput(signal, hold);
    }

    // Получаем сигнал пользователя на основе нажатой клавиши
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

    // Обрабатываем действия пользователя
    void userInput(UserAction_t signal, bool hold) {
        SnakeInfo_t snake_info = snake->GetSnakeInfo();

        typedef void (Snake::*action)();
        action fsm_simple[6] = { nullptr, &Snake::Spawn, nullptr, &Snake::Shifting, &Snake::GameOver, &Snake::ExitState };
        action fsm_table[2][9] = {
            { &Snake::StartGame, nullptr, &Snake::ExitState, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
            { &Snake::Check, &Snake::GamePause, &Snake::ExitState, &Snake::MoveLeft, &Snake::MoveRight, &Snake::MoveUp, &Snake::MoveDown, nullptr, &Snake::Check }
        };

        action act;
        if (snake_info.state != MOVING && snake_info.state != START) {
            act = fsm_simple[snake_info.state];
        } else {
            int state = (snake_info.state == MOVING) ? 1 : 0;
            act = fsm_table[state][signal];
        }

        if (act) {
            (snake->*act)();
        }
    }

    // Метод для обновления состояния игры
    void updateGame() {
        if (!isRunning) return;

        // Обновляем состояние игры и отображаем его через view
        GameInfo_t game_info = snake->updateCurrentState();
        view->UpdateView(game_info);

        // Проверка на конец игры
        SnakeInfo_t snake_info = snake->GetSnakeInfo();
        if (snake_info.state == GAMEOVER) {
            isRunning = false;
            view->DisplayGameOver(game_info);
        } else if (game_info.score >= 200) {
            isRunning = false;
            view->DisplayGameWin();
        }
    }
};
