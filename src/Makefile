CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror
LDFLAGS =  -lgtest -lgtest_main 

SRC = brick_game/tetris/ModelTetris.cpp brick_game/snake/ModelSnake.cpp brick_game/Game.h brick_game/defines.h
TESTS = tests/TetrisTest.cpp tests/SnakeTest.cpp

all: install

# Cборка QtGame
build-qt:
	cd main && qmake QtGame.pro && $(MAKE)

# Сборка сonsoleGame
build-console:
	cd main && qmake ConsoleGame.pro && $(MAKE)

clean:
	cd main && qmake QtGame.pro && $(MAKE) clean
	cd main && qmake ConsoleGame.pro && $(MAKE) clean
	rm -f main/Makefile main/.qmake.*
	rm -f tests/*.out

.PHONY: all build-qt build-console clean

test: 
	$(CXX) $(CXXFLAGS) $(SRC) $(TESTS)  -o resultTest $(LDFLAGS)
	./resultTest

dvi:
	doxygen Doxyfile
	open result/html/index.html

dist:
	tar -czf ./out/tetris.install.tar.gz ./* 

install: build-qt build-console

uninstall: clean
	rm ./bin/*

tetris:
	./bin/qtGame tetris

tetris_c:
	./bin/consoleGame tetris

snake:
	./bin/qtGame

snake_c:
	./bin/consoleGame

clng:
	clang-format -style=google -i ./brick_game/*.* ./brick_game/*/*.* ./gui/*/*.* ./main/*.cpp ./tests/*.cpp

clng_test:
	clang-format -style=google -n ./brick_game/*.* ./brick_game/*/*.* ./gui/*/*.* ./main/*.cpp ./tests/*.cpp

git:
	git add ./brick_game/*.* ./brick_game/*/*.* ./gui/*/*.* ./main/*.cpp ./tests/*.cpp Makefile
	git commit -m "release 1.0"
	git push origin develop