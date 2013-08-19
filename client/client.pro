#
# Qxmlrpc Client
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

#DEFINES += QT_NO_OPENSSL
INCLUDEPATH += ../utils/src


CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
TARGET = xmlrpcclientd
DEFINES += HTTP_CLIENT_DEBUG TRACE_HTTP DEBUG_PROTOCOL
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
TARGET = xmlrpcclient
}


include(client.pri)



