#include <QWidget>
#include <QTimer>
#include "../../brick_game/snake/QtControllerSnake.h"
#include "../../brick_game/snake/ModelSnake.h"

class SnakeView : public QWidget {
    Q_OBJECT

public:
    explicit SnakeView(Snake* snakeModel, QWidget* parent = nullptr)
        : QWidget(parent), snake(snakeModel) {
        controller = new SnakeController(snakeModel, this);

        // Создаем таймер для игрового цикла
        gameTimer = new QTimer(this);
        connect(gameTimer, &QTimer::timeout, this, &SnakeView::gameLoop);
        gameTimer->start(1000 / 60); // Частота обновления 60 FPS
    }

private:
    Snake* snake;
    SnakeController* controller;
    QTimer* gameTimer;

    // Основной игровой цикл
    void gameLoop() {
        SnakeInfo_t snake_info = snake->GetSnakeInfo();
        GameInfo_t game_info = snake->updateCurrentState();

        if (snake_info.state == MOVING || snake_info.state == START) {
            // Обновляем поле при движении или старте
            SumSnake(&game_info, snake_info);
            updateView(game_info);
        }

        // Обрабатываем паузу
        if (game_info.pause == 1) {
            displayPause();
        }

        // Проверяем победу или завершение игры
        if (game_info.score >= 200) {
            displayGameWin();
            gameTimer->stop(); // Останавливаем игру
        }

        if (snake_info.state == GAMEOVER) {
            displayGameOver();
            gameTimer->stop(); // Останавливаем игру
        }

        if (snake_info.state == EXIT_STATE) {
            close(); // Закрываем окно
        }
    }

    void updateView(const GameInfo_t& game_info) {
        // Логика обновления отображения игры (отрисовка)
    }

    void displayPause() {
        // Отображаем экран паузы
    }

    void displayGameOver() {
        // Отображаем "Game Over"
    }

    void displayGameWin() {
        // Отображаем сообщение о победе
    }

    void SumSnake(GameInfo_t *game_info, SnakeInfo_t snake_info) {
        int x, y;
        for (const auto& point : snake_info.snake->points) {
            x = point.x;
            y = point.y;
            game_info->field[x][y] = 1;
        }

        // Для отображения яблока другим цветом
        x = snake_info.apple->x;
        y = snake_info.apple->y;
        game_info->field[x][y] = 2;
    }
};
