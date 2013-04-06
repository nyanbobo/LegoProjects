#-------------------------------------------------
#
# Project created by QtCreator 2013-03-23T20:55:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCVBluetooth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bluetoothwrapper.cpp

HEADERS  += mainwindow.h \
    bluetoothwrapper.h

FORMS    += mainwindow.ui

LIBS    += `pkg-config --libs opencv`
LIBS    += -lbluetooth
