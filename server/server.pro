#
# Qxmlrpc Server
#
TEMPLATE = lib
CONFIG += staticlib
QT = core network xml

greaterThan(QT_MAJOR_VERSION, 4){
DESTDIR = ../libq5
}
else {
DESTDIR = ../lib
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


