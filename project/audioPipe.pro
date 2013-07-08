#-------------------------------------------------
#
# Project created by QtCreator 2013-06-13T07:06:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = audioPipe
TEMPLATE = app


SOURCES += ..\src\main.cpp\
        ..\src\mainwindow.cpp \
    ../src/getTimeMs64.cpp \
    ../src/GraphicsWorker.cpp \
    ../src/DataFile.cpp

HEADERS  += ..\src\mainwindow.h \
    ../src/GraphicsWorker.h \
    ../src/DataFile.h

FORMS    += ..\ui\mainwindow.ui

RESOURCES += \
    ..\resources\icons.qrc


