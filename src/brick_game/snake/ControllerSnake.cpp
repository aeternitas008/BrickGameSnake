#include "ControllerSnake.h"

void SnakeController::GameLoop() {
        Snake snake;
        GameInfo_t game_info;
        SnakeInfo_t snake_info;

        view->DisplayOverlay();
        while (no_break) {
            game_info = snake.updateCurrentState();
            snake_info = snake.GetSnakeInfo();

            UserAction_t signal = UserAction_t::Nosig;
            if (snake_info.state == MOVING || snake_info.state == START) {
                int key = GET_USER_INPUT;
                signal = input_handler->GetSignal(key);  // Получаем сигнал через интерфейс ввода
            }

            if (snake_info.state == START && signal == UserAction_t::Start)
                view->DisplayField();

            if (game_info.pause == 1) {
                view->DisplayPause();
                int key = GET_USER_INPUT;
                input_handler->GetSignal(key);
                if (signal == UserAction_t::Pause) {
                    view->ClearPause();
                    snake.GamePause();
                    // snake.GameResume();
                }
            }

            if (game_info.pause != 1) {
                snake.userInput(signal, 0);
            }

            game_info = snake.updateCurrentState();

            if (snake_info.state != START) {
                // snake.SumSnake(&game_info, snake_info);
                view->UpdateView(game_info);
            }

            if (game_info.score == 200) {
                view->DisplayGameWin();
                no_break = false;
            }

            if (snake_info.state == GAMEOVER) {
                view->DisplayGameOver(game_info);
                no_break = false;
            }

            if (snake_info.state == EXIT_STATE) {
                no_break = false;
            }
        }
    }