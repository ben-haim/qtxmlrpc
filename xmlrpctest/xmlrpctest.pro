
TEMPLATE = app
DESTDIR = bin

QT += network xml


INCLUDEPATH += ../server/src
INCLUDEPATH += ../client/src
LIBS += -L../client/lib
LIBS += -L../server/lib

SOURCES += ./src/main.cpp ./src/server.cpp ./src/client.cpp
HEADERS += ./src/server.h ./src/client.h

CONFIG += console

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
}

CONFIG(debug, debug|release) {
LIBS += -lxmlrpcclientd
LIBS += -lxmlrpcserverd
}
else {
LIBS += -lxmlrpcclient
LIBS += -lxmlrpcserver
}
