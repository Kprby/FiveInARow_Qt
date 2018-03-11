#-------------------------------------------------
#
# Project created by QtCreator 2016-12-06T15:14:32
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FIR
TEMPLATE = app


SOURCES += main.cpp\
    chessboard.cpp \
    mainwindow.cpp \
    windialog.cpp \
    instructiondialog.cpp \
    ai.cpp

HEADERS  += chessboard.h \
    mainwindow.h \
    windialog.h \
    instructiondialog.h \
    ai.h \
    chess.h

FORMS += \
    windialog.ui \
    instructiondialog.ui
