#include "../brick_game/snake/ModelSnake.h"
#include "gtest/gtest.h"
#include <deque>

// class Snake;

class SnakeTest : public ::testing::Test {

    // friend class Snake;

protected:
    GameInfo_t game_info;
    Snake snake_game;
    Snake_t snake;
    Position_t apple;
    timespec time;

    void SetUp() override {
        game_info = {};
        snake = {};
        apple = {};
        time = {};

        snake.points = {{5, 5}, {5, 4}, {5, 3}};
        snake.direction = RIGHT_DIRECTION;

        apple.x = 8;
        apple.y = 8;

        snake_game.snake_info_ = new SnakeInfo_t{&time, &snake, &apple};
        snake_game.game_info_ = &game_info;
    }

    void TearDown() override {
        delete snake_game.snake_info_;
    }
};

TEST_F(SnakeTest, SumSnakeUpdatesGameInfoField) {
    SnakeInfo_t snake_info = snake_game.getSnakeInfo();
    snake_game.SumSnake(&game_info, snake_info);

    EXPECT_EQ(game_info.field[5][5], 1);
    EXPECT_EQ(game_info.field[5][4], 1);
    EXPECT_EQ(game_info.field[5][3], 1);
    EXPECT_EQ(game_info.field[8][8], 2);
}

TEST_F(SnakeTest, IsBodySnakeReturnsTrueForBody) {
    Position_t point = {5, 5};
    EXPECT_EQ(snake_game.IsBodySnake(point), 1);
}

TEST_F(SnakeTest, IsBodySnakeReturnsFalseForNonBody) {
    Position_t point = {10, 10};
    EXPECT_EQ(snake_game.IsBodySnake(point), 0);
}

TEST_F(SnakeTest, GetRandomCoordinatesAvoidsSnakeBody) {
    snake_game.GetRandomCoordiantes();
    Position_t apple_pos = {apple.x, apple.y};

    EXPECT_EQ(snake_game.IsBodySnake(apple_pos), 0);
    EXPECT_GE(apple.x, 0);
    EXPECT_LT(apple.x, 20);
    EXPECT_GE(apple.y, 0);
    EXPECT_LT(apple.y, 10);
}

TEST_F(SnakeTest, IsCollisionDetectsWallCollision) {
    Position_t point = {-1, 5};
    EXPECT_EQ(snake_game.IsCollision(point), 1);

    point = {20, 5};
    EXPECT_EQ(snake_game.IsCollision(point), 1);

    point = {5, -1};
    EXPECT_EQ(snake_game.IsCollision(point), 1);

    point = {5, 10};
    EXPECT_EQ(snake_game.IsCollision(point), 1);
}

TEST_F(SnakeTest, IsCollisionDetectsSelfCollision) {
    Position_t point = {5, 4};
    EXPECT_EQ(snake_game.IsCollision(point), 1);
}

TEST_F(SnakeTest, IsEatingReturnsTrueIfEatingApple) {
    Position_t point = {8, 8};
    EXPECT_EQ(snake_game.IsEating(point), 1);
}

TEST_F(SnakeTest, IsEatingReturnsFalseIfNotEatingApple) {
    Position_t point = {5, 5};
    EXPECT_EQ(snake_game.IsEating(point), 0);
}

TEST_F(SnakeTest, MoveForwardUpdatesSnakePosition) {
    snake_game.MoveForward();

    EXPECT_EQ(snake.points.size(), 3);
    EXPECT_EQ(snake.points.front().x, 5);
    EXPECT_EQ(snake.points.front().y, 6);
}

TEST_F(SnakeTest, MoveForwardHandlesEatingApple) {
    apple.x = 6;
    apple.y = 5;

    snake.direction = DOWN_DIRECTION;
    snake_game.MoveForward();

    EXPECT_EQ(snake.points.size(), 4);
    EXPECT_EQ(game_info.score, 1);
    EXPECT_EQ(game_info.state, SPAWN);
}

TEST_F(SnakeTest, MoveForwardHandlesCollision) {
    snake.points.push_front({5, 6});
    snake_game.MoveForward();

    EXPECT_EQ(game_info.state, GAMEOVER);
}

TEST_F(SnakeTest, UserInputHandlesMoveLeft) {
    snake_game.userInput(UserAction_t::Left, false);
    EXPECT_EQ(snake.direction, LEFT_DIRECTION);
}

TEST_F(SnakeTest, UserInputHandlesStartGame) {
    game_info.state = START;
    snake_game.userInput(UserAction_t::Start, false);

    EXPECT_EQ(game_info.state, SPAWN);
}
