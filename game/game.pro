#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T15:01:07
#
#-------------------------------------------------

QT += core gui network opengl remoteobjects

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app
CONFIG += c++14

include(../common/common.pri)

REPC_SOURCE = $$PWD/player.rep
REPC_REPLICA += $$PWD/player.rep \
                $$PWD/../common/counter.rep

SOURCES += main.cpp\
        mainwindow.cpp \
        display.cpp \
        tank.cpp \
        missile.cpp \
        score.cpp \
        obstacle.cpp \
    gameconnector.cpp \
    basemapobject.cpp \
    controller.cpp

HEADERS  += mainwindow.h \
        display.h \
        tank.h \
        missile.h \
        score.h \
        obstacle.h \
    gameconnector.h \
    basemapobject.h \
    controller.h

FORMS    += mainwindow.ui
