
TEMPLATE = app
DESTDIR = ../bin

QT += network xml


INCLUDEPATH += ../../server/src
INCLUDEPATH += ../../client/src
INCLUDEPATH += ../../utils/src

greaterThan(QT_MAJOR_VERSION, 4){
LIBS += -L../../libq5
}
else {
LIBS += -L../../lib
}

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
LIBS += -lxmlrpcclientd
LIBS += -lxmlrpcserverd
LIBS += -lxmlrpcutilsd
}
else {
LIBS += -lxmlrpcclient
LIBS += -lxmlrpcserver
LIBS += -lxmlrpcutils
}
