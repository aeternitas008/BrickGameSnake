CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror
LDFLAGS =  -lgtest -lgtest_main 

SRC = brick_game/tetris/ModelTetris.cpp brick_game/snake/ModelSnake.cpp brick_game/Game.h brick_game/defines.h
TESTS = tests/TetrisTest.cpp tests/SnakeTest.cpp
# MAIN_TEST = tests/main.cpp
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

test: 
	$(CXX) $(CXXFLAGS) $(SRC) $(TESTS)  -o resultTest $(LDFLAGS)
	./resultTest

dvi:

dest:

install:

uninstall:

clng:
	clang-format -style=google src/*/*/*.*