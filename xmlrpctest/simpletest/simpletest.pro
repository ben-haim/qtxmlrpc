
TEMPLATE = app
DESTDIR = ../bin

QT += network xml testlib

INCLUDEPATH += ../../qtxmlrpc
INCLUDEPATH += ../../qtxmlrpc/server
INCLUDEPATH += ../../qtxmlrpc/client
INCLUDEPATH += ../../qtxmlrpc/utils

LIBS+= -L../../lib/$$basename(QMAKESPEC)/$$QT_VERSION

SOURCES += ./src/main.cpp ./src/server.cpp ./src/client.cpp
HEADERS += ./src/server.h ./src/client.h

CONFIG += console

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
TARGET = simpletestd
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
TARGET = simpletest
}

CONFIG(debug, debug|release) {
LIBS += -lqtxmlrpcd
}
else {
LIBS += -lqtxmlrpc
}
