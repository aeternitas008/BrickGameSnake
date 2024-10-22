CC = gcc
FLAG_C = -c
FLAG_ER = -Wall -Werror -Wextra -std=c11
TET_LIB_C = ./brick_game/tetris/model.c
view_C = ./gui/cli/view.c
MAIN_C = ./brick_game/tetris/main.c
TET_LIB_O = brick_game/tetris/model.o
CONTR_C = brick_game/tetris/controller.c
CONTR_O = brick_game/tetris/controller.o
view_O = gui/cli/view.o
FLAG_TEST = -lcheck -lm  -I/opt/homebrew/include

all: clean uninstall tetris

tetris: tetris_lib.a $(view_O) $(MAIN_C) $(CONTR_O)
	mkdir ./game
	$(CC) $(FLAG_ER) -o ./game/tetris $(MAIN_C) $(view_O) $(CONTR_O) tetris_lib.a -lncurses

install: tetris
	mkdir ./game
	cp tetris ./game

uninstall:
	-rm -rf ./game

tetris_lib.a: $(TET_LIB_O) $(view_O) $(CONTR_O)
	ar rc tetris_lib.a $(TET_LIB_O) $(view_O)
	ranlib tetris_lib.a

$(TET_LIB_O): $(TET_LIB_C)
	$(CC) $(FLAG_C) $(FLAG_ER) $(TET_LIB_C) -o $(TET_LIB_O)

$(view_O): $(view_C)
	$(CC) $(FLAG_C) $(FLAG_ER) $(view_C) -o $(view_O)

clean: uninstall 
	-rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg
	-rm -f test tests greport tetris.install.tar.gz
	-rm -rf report test.d* tetris out/* doxygen/html doxygen/result
	find . -name "*.o" -type f -delete


test: clean tetris_lib.a
	$(CC) -g --coverage $(FLAG_ER) test.c -o ./out/test tetris_lib.a -lncurses $(FLAG_TEST)
	./out/test

gcov_report: test
	$(CC) --coverage $(TET_LIB_C) $(view_C) test.c -o ./out/greport $(FLAG_TEST) -lncurses
	./out/greport
	lcov -t "test" -o tetris.info --no-external -c -d .
	genhtml -o ./report tetris.info
	open ./report/index.html

dist:
	tar -czf ./out/tetris.install.tar.gz ./* 

dvi:
	cd doxygen && doxygen

style:
	find .. -name "*.c" -exec clang-format -i -style=google {} \;
	find .. -name "*.h" -exec clang-format -i -style=google {} \;

style_test:
	cp ../materials/linters/.clang-format .
	find .. -name "*.c" -exec clang-format -n -style=google {} \;
	find .. -name "*.h" -exec clang-format -n -style=google {} \;
	rm -rf .clang-format

