
TEMPLATE = app
DESTDIR = ../bin

QT += network xml testlib

INCLUDEPATH += ../../qtxmlrpc

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

QTXMLLIBSUFFIX = ""
win32{
QTXMLLIBSUFFIX = "0"
}
CONFIG(debug, debug|release) {
LIBS += -lqtxmlrpcd$$QTXMLLIBSUFFIX
}
else {
LIBS += -lqtxmlrpc$$QTXMLLIBSUFFIX
}

unix: QMAKE_LFLAGS += -Wl,-rpath,'../../lib/$$basename(QMAKESPEC)/$$QT_VERSION'
unix: QMAKE_LFLAGS += -Wl,-rpath,'.'

