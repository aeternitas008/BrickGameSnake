TEMPLATE = app

CONFIG += console
CONFIG += c++17 qt
CONFIG -= app_bundle

QT += core gui widgets

#DEFINES += USE_QT

LIBS += -lncurses

DESTDIR = $$PWD/../bin/
TARGET = consoleGame

SOURCES += consoleMain.cpp \
           ../brick_game/GameController.cpp \
           ../brick_game/snake/ModelSnake.cpp \
           ../brick_game/tetris/ModelTetris.cpp \
           ../gui/cli/ConsoleView.cpp \

HEADERS += ../brick_game/GameController.h \
           ../brick_game/snake/ModelSnake.h \
           ../brick_game/tetris/ModelTetris.h \
           ../gui/cli/ConsoleView.h \
           ../brick_game/Game.h \