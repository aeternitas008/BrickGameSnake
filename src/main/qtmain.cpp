#include "../gui/desktop/BrickGame.h"
#include "../brick_game/tetris/QtTetrisContr.h"
#include "../brick_game/snake/QtSnakeContr.h"
#include <QApplication>
#include <QString>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Проверяем аргументы командной строки
    QString gameType = "tetris";  // По умолчанию Tetris
    if (argc > 1) {
        gameType = QString(argv[1]).toLower();
    }

    BrickGame view;
    TetrisController* controller1 = nullptr;
    SnakeController* controller2 = nullptr;

    // Tetris tetris;  // Создаём экземпляр Tetris
    // Snake snake;  // Создаём экземпляр Snake
    if (gameType == "tetris") {
        Tetris tetris;  // Создаём экземпляр Tetris
        controller1 = new TetrisController(&view, tetris);  // Создаём контроллер как динамический объект
        std::cout << "Starting Tetris..." << std::endl;
    } else if (gameType == "snake") {
        Snake snake;  // Создаём экземпляр Snake
        controller2 = new SnakeController(&view, snake);  // Создаём контроллер как динамический объект
        std::cout << "Starting Snake..." << std::endl;
    } else {
        std::cerr << "Unknown game type: " << gameType.toStdString() << std::endl;
        std::cerr << "Usage: " << argv[0] << " [tetris|snake]" << std::endl;
        return 1;  // Завершаем программу с ошибкой
    }

    view.show();  // Показываем окно игры

    // Здесь контроллер будет жить, пока приложение работает
    return app.exec();
}
