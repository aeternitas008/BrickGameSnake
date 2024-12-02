TEMPLATE = app

CONFIG += console
CONFIG += c++17 qt
CONFIG -= app_bundle

QT += core gui widgets

#DEFINES += USE_QT

LIBS += -lncurses

DESTDIR = $$PWD/../bin/
TARGET = qtGame

SOURCES += qtmain.cpp \
           ../gui/desktop/BrickGame.cpp \
           ../brick_game/tetris/ModelTetris.cpp \
           ../brick_game/tetris/ControllerTetris.cpp \
           ../brick_game/snake/ModelSnake.cpp \
           ../brick_game/snake/ControllerSnake.cpp \
           ../brick_game/tetris/QtTetrisContr.cpp \
           ../brick_game/snake/QtSnakeContr.cpp \

HEADERS += ../gui/desktop/BrickGame.h \
           ../brick_game/tetris/ModelTetris.h \
           ../brick_game/tetris/ControllerTetris.h \
           ../brick_game/snake/ModelSnake.h \
           ../brick_game/snake/ControllerSnake.h \
           ../brick_game/InputHandler.h \
           ../brick_game/tetris/QtTetrisContr.h \
           ../brick_game/snake/QtSnakeContr.h \