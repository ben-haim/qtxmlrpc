
TEMPLATE = app
DESTDIR = ../bin

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INCLUDEPATH += ../../server/src
INCLUDEPATH += ../../client/src
INCLUDEPATH += ../../utils/src

LIBS+= -L../../lib/$$basename(QMAKESPEC)/$$QT_VERSION

SOURCES += ./src/main.cpp ./src/test_server.cpp
HEADERS += ./src/test_server.h

CONFIG += console

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
TARGET = testsrvd
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
TARGET = testsrv
}

CONFIG(debug, debug|release) {
LIBS += -lxmlrpcserverd
LIBS += -lxmlrpcutilsd
}
else {
LIBS += -lxmlrpcserver
LIBS += -lxmlrpcutils
}
