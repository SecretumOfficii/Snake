#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T16:37:08
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    zznake.cpp

HEADERS  += mainwindow.h \
    zznake.h

RESOURCES += \
    res.qrc
