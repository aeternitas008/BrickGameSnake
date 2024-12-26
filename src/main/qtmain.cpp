#include <QApplication>
#include <QString>
#include <iostream>

#include "../brick_game/tetris/ModelTetris.h"
#include "../gui/desktop/BrickGame.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QString gameType = "snake";
  if (argc > 1) {
    gameType = QString(argv[1]).toLower();
  }

  BrickGame* view = nullptr;
  Game* game = nullptr;
  if (gameType == "tetris") {
    game = new Tetris;
    std::cout << "Starting Tetris..." << std::endl;
  } else if (gameType == "snake") {
    game = new Snake;
    std::cout << "Starting Snake..." << std::endl;
  } else {
    std::cerr << "Unknown game type: " << gameType.toStdString() << std::endl;
    std::cerr << "Usage: " << argv[0] << " [tetris|snake]" << std::endl;
    return 1;
  }

  view = new BrickGame(*game);
  view->show();
  return app.exec();
}
