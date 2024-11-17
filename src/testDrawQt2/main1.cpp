#include "BrickGame.h"
#include "TetrisController.h"
#include <QApplication>

#include "../brick_game/snake/ModelSnake.h"
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Tetris tetris;  // Единственный экземпляр Tetris
    // Snake snake;  // Единственный экземпляр Tetris
    BrickGame view(tetris);
    TetrisController controller(&view, tetris);

    view.show();
    return app.exec();
}
