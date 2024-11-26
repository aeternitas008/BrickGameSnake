// SnakeController.h
#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H

// #include "../../gui/ViewInterface.h"
#include "../InputHandler.h"
#include "ModelSnake.h"
#include "../../gui/cli/ConsoleView.h"

class SnakeController {
public:
    SnakeController(InputHandler* input, ViewInterface* view) 
        : input_handler(input), view(view), no_break(true) {}

    void GameLoop() {
        Snake snake;
        GameInfo_t game_info;
        SnakeInfo_t snake_info;
        int hold = 0;

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
                snake.userInput(signal, hold);
            }

            game_info = snake.updateCurrentState();

            if (snake_info.state != START) {
                SumSnake(&game_info, snake_info);
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

private:
    InputHandler* input_handler;  // Источник ввода
    ViewInterface* view;          // Интерфейс отображения
    bool no_break;

    // вынести в бизнес логику? или другие способы
    void SumSnake(GameInfo_t *game_info, SnakeInfo_t snake_info) {
        int x, y;
        for (int i = 0; i < snake_info.snake->points.size(); i++) {
            x = snake_info.snake->points[i].x;
            y = snake_info.snake->points[i].y;
            game_info->field[x][y] = 1;
        }
        x = snake_info.apple->x;
        y = snake_info.apple->y;
        game_info->field[x][y] = 2;  // для отображения другим цветом
    }
};

#endif  // SNAKE_CONTROLLER_H
