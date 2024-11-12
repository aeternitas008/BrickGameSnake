#include <QApplication>
#include "TetrisGame.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TetrisGame tetrisGame;
    tetrisGame.show();

    return app.exec();
}
