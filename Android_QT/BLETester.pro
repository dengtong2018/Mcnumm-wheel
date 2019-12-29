#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T07:38:29
#
#-------------------------------------------------

QT       += core gui bluetooth
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BLETester
TEMPLATE = app


SOURCES += main.cpp\
    joystickprototype.cpp \
        mainwindow.cpp \
    bleinterface.cpp \
    message.cpp

HEADERS  += mainwindow.h \
    bleinterface.h \
    joystickprototype.h \
    lib-qt-qml-tricks/src/qqmlhelpers.h \
    message.h

FORMS    += mainwindow.ui \
    joystickprototype.ui

RESOURCES += \
    resources.qrc \
    resources.qrc

DISTFILES += \
    refresh.png
