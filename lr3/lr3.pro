#-------------------------------------------------
#
# Project created by QtCreator 2013-12-01T20:32:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lr3
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    data/img.cpp \
    model/Net.cpp

HEADERS  += MainWindow.h \
    data/img.h \
    model/Net.h

FORMS    += MainWindow.ui

QMAKE_CXXFLAGS += -std=c++11
