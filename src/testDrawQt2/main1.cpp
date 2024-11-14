#include <QApplication>
#include "BrickGame.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BrickGame tetrisGame;
    tetrisGame.show();

    return app.exec();
}
