#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T15:01:07
#
#-------------------------------------------------

QT += core gui network opengl remoteobjects

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app


REPC_SOURCE = $$PWD/player.rep
REPC_REPLICA = $$PWD/player.rep

SOURCES += main.cpp\
        mainwindow.cpp \
        display.cpp \
        tank.cpp \
        missile.cpp \
        score.cpp \
        obstacle.cpp \
    game.cpp

HEADERS  += mainwindow.h \
        display.h \
        tank.h \
        missile.h \
        score.h \
        obstacle.h \
    game.h

FORMS    += mainwindow.ui
