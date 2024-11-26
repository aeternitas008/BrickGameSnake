TEMPLATE = app

CONFIG += console
CONFIG += c++17 qt
CONFIG -= app_bundle

QT += core gui widgets

#DEFINES += USE_QT

LIBS += -lncurses

DESTDIR = $$PWD/bin/
#TARGET = brickGame2

SOURCES += main1.cpp \
           BrickGame.cpp \
           ../../brick_game/tetris/ModelTetris.cpp \
           ../../brick_game/tetris/ControllerTetris.cpp \
           TetrisController.cpp \

HEADERS += BrickGame.h \
           ../../brick_game/tetris/ModelTetris.h \
           ../../brick_game/tetris/ControllerTetris.h \
           InputHandler.h \
           TetrisController.h \