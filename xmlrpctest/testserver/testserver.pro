
TEMPLATE = app
DESTDIR = bin

QT += network xml


INCLUDEPATH += ../../server/src
INCLUDEPATH += ../../client/src
INCLUDEPATH += ../../utils/src

LIBS += -L../../client/lib
LIBS += -L../../server/lib
LIBS += -L../../utils/lib

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
