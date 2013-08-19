
TEMPLATE = app
DESTDIR = ../bin

QT += network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INCLUDEPATH += ../../server/src
INCLUDEPATH += ../../client/src
INCLUDEPATH += ../../utils/src

greaterThan(QT_MAJOR_VERSION, 4){
win32-g++{
    LIBS+= -L../../libq5/mingw
}
else {
    LIBS+= -L../../libq5
}#win32-g++
}#greaterThan(QT_MAJOR_VERSION, 4)
else {
win32-g++{
    LIBS+= -L../../lib/mingw
}
else {
    LIBS+= -L../../lib
}#win32-g++
}

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
