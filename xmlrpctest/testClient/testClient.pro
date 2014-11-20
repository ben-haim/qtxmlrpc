#-------------------------------------------------
#
# Project created by QtCreator 2012-01-11T19:02:39
#
#-------------------------------------------------

TEMPLATE = app
DESTDIR = ../bin

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

UI_HEADERS_DIR = build
UI_DIR         = build

INCLUDEPATH += ../../qtxmlrpc

LIBS+= -L../../lib/$$basename(QMAKESPEC)/$$QT_VERSION

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
TARGET = testClientd
CONFIG += console
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
TARGET = testClient
}

CONFIG(debug, debug|release) {
LIBS += -lqtxmlrpcd
}
else {
LIBS += -lqtxmlrpc
}


SOURCES += ./src/main.cpp\
        ./src/mainwindow.cpp

HEADERS  += ./src/mainwindow.hpp

FORMS    += ./src/mainwindow.ui

RESOURCES += \
    app_res.qrc

unix: QMAKE_LFLAGS += -Wl,-rpath,'../../lib/$$basename(QMAKESPEC)/$$QT_VERSION'
unix: QMAKE_LFLAGS += -Wl,-rpath,'.'
