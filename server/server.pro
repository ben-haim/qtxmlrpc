#
# Qxmlrpc Server
#
TEMPLATE = lib
CONFIG += staticlib
QT = core network xml

greaterThan(QT_MAJOR_VERSION, 4){
win32-g++{
    DESTDIR = ../libq5/mingw
}
else {
    DESTDIR = ../libq5
}#win32-g++
}#greaterThan(QT_MAJOR_VERSION, 4)
else {
win32-g++{
    DESTDIR = ../lib/mingw
}
else {
    DESTDIR = ../lib
}#win32-g++
}

INCLUDEPATH += ../utils/src

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
TARGET = xmlrpcserverd
DEFINES += DEBUG_XMLRPC DEBUG_PROTOCOL
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
TARGET = xmlrpcserver
}

include(server.pri)


