XX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror
LDFLAGS = -lgtest -lpthread

MODELS = brick_game/tetris/ModelTetris.cpp brick_game/snake/ModelSnake.cpp
TESTS = tests/TetrisTest.cpp tests/SnakeTest.cpp
MAIN_TEST = tests/main.cpp
# Имя основного Makefile
all: build-qt build-console

# Команды для сборки QtGame
build-qt:
	cd main && qmake QtGame.pro && $(MAKE)

# Команды для сборки ConsoleGame
build-console:
	cd main && qmake ConsoleGame.pro && $(MAKE)

# Очистка сборки
clean:
	cd main && qmake QtGame.pro && $(MAKE) clean
	cd main && qmake ConsoleGame.pro && $(MAKE) clean
	rm -f tests/*.out

.PHONY: all build-qt build-console clean

test: $(MODELS) $(TESTS) $(MAIN_TEST) 
	$(CXX) $(CXXFLAGS) $^  -o tests $(LDFLAGS)
	./tests

dvi:

dest:

install:

uninstall:

clng:
	clang-format -style=google src/*/*/*.*