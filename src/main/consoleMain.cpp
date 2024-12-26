#include <iostream>
#include <string>

#include "../brick_game/GameController.h"
#include "../brick_game/snake/ModelSnake.h"
#include "../brick_game/tetris/ModelTetris.h"
#include "../gui/cli/ConsoleView.h"

int main(int argc, char* argv[]) {
  std::string gameChoice = "snake";
  if (argc > 1) {
    gameChoice = argv[1];
  }
  ConsoleView view;

  setlocale(LC_CTYPE, "");
  WIN_INIT(INITIAL_TIMEOUT);
  if (gameChoice == "snake") {
    Game* game = new Snake;
    GameController controller(&view, *game);
    controller.GameLoop();
  } else if (gameChoice == "tetris") {
    Game* game = new Tetris;
    GameController controller(&view, *game);
    controller.GameLoop();
  } else {
    std::cout << "Unknown game: " << gameChoice << "\n";
  }
  endwin();
  return SUCCESS;
}