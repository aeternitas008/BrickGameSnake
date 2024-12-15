#include "../gui/desktop/BrickGame.h"
#include "../brick_game/tetris/ModelTetris.h"
#include <QApplication>
#include <QString>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Проверяем аргументы командной строки
    QString gameType = "snake";  // По умолчанию Tetris
    if (argc > 1) {
        gameType = QString(argv[1]).toLower();
    }

    BrickGame* view = nullptr;
    Game* game = nullptr;
    if (gameType == "tetris") {
        game = new Tetris;  // Создаём экземпляр Tetris
        std::cout << "Starting Tetris..." << std::endl;
    } else if (gameType == "snake") {
        game = new Snake;  // Создаём экземпляр Snake
        std::cout << "Starting Snake..." << std::endl;
    } else {
        std::cerr << "Unknown game type: " << gameType.toStdString() << std::endl;
        std::cerr << "Usage: " << argv[0] << " [tetris|snake]" << std::endl;
        return 1;  // Завершаем программу с ошибкой
    }

    view = new BrickGame(*game);
    // QtGameController* controller = new QtGameController(view, *game);
    view->show();  // Показываем окно игры

    // Здесь контроллер будет жить, пока приложение работает
    return app.exec();
}
