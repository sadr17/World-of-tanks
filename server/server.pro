#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T21:38:53
#
#-------------------------------------------------

QT       += core gui network remoteobjects

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app

REPC_SOURCE = ../common/counter.rep

include(../common/common.pri)

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \

FORMS += mainwindow.ui
