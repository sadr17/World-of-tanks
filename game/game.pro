#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T15:01:07
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        display.cpp \
        tank.cpp \
        missile.cpp

HEADERS  += mainwindow.h \
    display.h \
    tank.h \
    missile.h

FORMS    += mainwindow.ui
