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


INCLUDEPATH += ../../server/src
INCLUDEPATH += ../../client/src
INCLUDEPATH += ../../utils/src

greaterThan(QT_MAJOR_VERSION, 4){
LIBS += -L../../libq5
}
else {
LIBS += -L../../lib
}

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
LIBS += -lxmlrpcclientd
LIBS += -lxmlrpcutilsd
}
else {
LIBS += -lxmlrpcclient
LIBS += -lxmlrpcutils
}


SOURCES += ./src/main.cpp\
        ./src/mainwindow.cpp

HEADERS  += ./src/mainwindow.hpp

FORMS    += ./src/mainwindow.ui
