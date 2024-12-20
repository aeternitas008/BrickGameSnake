#include <gtest/gtest.h>
#include "../brick_game/tetris/ModelTetris.h"
#include <array>

class TetrisTest : public ::testing::Test {
protected:
    Tetris tetris;

    void SetUp() override {
        auto tetris_info = tetris.getTetrisInfo();
        tetris_info.tetramino->point->x = 0;
        tetris_info.tetramino->point->y = 0;
        tetris_info.tetramino->last_tetramino = {-1, -1, -1, -1};
    }
};

TEST_F(TetrisTest, IsUnique_ReturnsTrueForUniqueTetramino) {
    std::array<int, 4> last_tetraminos = {-1, 2, 3, 4};
    EXPECT_TRUE(tetris.isUnique(1, last_tetraminos));
}

TEST_F(TetrisTest, IsUnique_ReturnsFalseForDuplicateTetramino) {
    std::array<int, 4> last_tetraminos = {1, 2, 3, 4};
    EXPECT_FALSE(tetris.isUnique(4, last_tetraminos));
}

TEST_F(TetrisTest, AddTypeTetramino_UpdatesLastTetraminosCorrectly) {
    auto tetris_info = tetris.getTetrisInfo();
    tetris_info.tetramino->last_tetramino = {1, 2, 3, 4};
    tetris.AddTypeTetramino(5);
    EXPECT_EQ(tetris_info.tetramino->last_tetramino, (std::array<int, 4>{2, 3, 4, 5}));
}

TEST_F(TetrisTest, AddTetraminoOnBoard_UpdatesFieldCorrectly) {
    auto tetris_info = tetris.getTetrisInfo();
    tetris_info.tetramino->figure[0][0] = 1;
    tetris_info.tetramino->point->x = 0;
    tetris_info.tetramino->point->y = 0;

    tetris.AddTetraminoOnBoard();
    auto game_info = tetris.updateCurrentState();
    EXPECT_EQ(game_info.field[0][0], 1);
}

TEST_F(TetrisTest, HasFullLine_ReturnsTrueForFullLine) {
    auto game_info = tetris.updateCurrentState();
    for (int y = 0; y < 10; ++y) {
        game_info.field[0][y] = 1;
    }
    int line = 0;
    tetris.setGameInfo(game_info);
    EXPECT_EQ(tetris.HasFullLine(&line), 0);
    EXPECT_EQ(line, 0);
}

TEST_F(TetrisTest, Spawn_InitializesTetraminoCorrectly) {
    tetris.Spawn();
    auto tetris_info = tetris.getTetrisInfo();
    EXPECT_EQ(tetris_info.tetramino->point->x, TETR_START_X);
    EXPECT_EQ(tetris_info.tetramino->point->y, TETR_START_Y);
}

TEST_F(TetrisTest, MoveDown_MovesTetraminoCorrectly) {
    auto tetris_info = tetris.getTetrisInfo();
    tetris_info.tetramino->point->x = 0;

    tetris.MoveDown();
    EXPECT_EQ(tetris_info.tetramino->point->x, 1);
}

TEST_F(TetrisTest, MoveRight_MovesTetraminoRight) {
    auto tetris_info = tetris.getTetrisInfo();
    tetris_info.tetramino->point->y = 0;

    tetris.MoveRight();
    EXPECT_EQ(tetris_info.tetramino->point->y, 1);
}

TEST_F(TetrisTest, MoveLeft_MovesTetraminoLeft) {
    auto tetris_info = tetris.getTetrisInfo();
    tetris_info.tetramino->point->y = 1;

    tetris.MoveLeft();
    EXPECT_EQ(tetris_info.tetramino->point->y, 0);
}

TEST_F(TetrisTest, CheckTetramino_DetectsCollision) {
    auto tetris_info = tetris.getTetrisInfo();
    auto game_info = tetris.updateCurrentState();

    game_info.field[1][0] = 1;
    tetris.setGameInfo(game_info);
    tetris_info.tetramino->figure[0][0] = 1;
    tetris_info.tetramino->point->x = 0;
    tetris_info.tetramino->point->y = 0;

    EXPECT_EQ(tetris.CheckTetramino(*tetris_info.tetramino), 1);
}

// TEST_F(TetrisTest, ClearFullLine_RemovesFilledLine) {
//     auto game_info = tetris.updateCurrentState();

//     for (int y = 0; y < 10; ++y) {
//         game_info.field[1][y] = 1;
//     }
//     tetris.setGameInfo(game_info);
//     tetris.Spawn();
//     tetris.userInput(Down, false);
//     tetris.Check();

//     game_info = tetris.updateCurrentState();
//     EXPECT_EQ(game_info.score, 100);
//     for (int y = 0; y < 10; ++y) {
//         EXPECT_EQ(game_info.field[1][y], 0);
//     }
// }

TEST_F(TetrisTest, GameOver_DetectsGameOver) {
    auto game_info = tetris.updateCurrentState();
    for (int y = 0; y < 10; ++y) {
        game_info.field[20][y] = 1;
    }
    tetris.setGameInfo(game_info);
    tetris.Spawn();
    game_info = tetris.updateCurrentState();

    EXPECT_EQ(game_info.state, GAMEOVER);
}