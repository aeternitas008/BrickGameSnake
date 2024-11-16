TEMPLATE = app

CONFIG += console
CONFIG += c++17 qt
CONFIG -= app_bundle

QT += core gui widgets

#DEFINES += USE_QT

LIBS += -lncurses

DESTDIR = $$PWD/bin/
TARGET = brickGame

SOURCES += main.cpp \
           brick_game/snake/ControllerSnake.cpp \
           brick_game/snake/ModelSnake.cpp \
           brick_game/tetris/ControllerTetris.cpp \
           brick_game/tetris/ModelTetris.cpp \
           gui/cli/ConsoleView.cpp \
           gui/desktop/GameWindow.cpp

HEADERS += brick_game/snake/ControllerSnake.h \
           brick_game/snake/ModelSnake.h \
           brick_game/tetris/ControllerTetris.h \
           brick_game/tetris/ModelTetris.h \
           gui/cli/ConsoleView.h \
           gui/desktop/GameWindow.hpp \
           gui/desktop/QtView.h \
           gui/ViewInterface.h