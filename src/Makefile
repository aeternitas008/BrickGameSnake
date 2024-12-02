# Имя основного Makefile
all: build-qt

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

.PHONY: all build-qt build-console clean