#
# Qxmlrpc Utils
#
TEMPLATE = lib
CONFIG += staticlib
QT = core xml network

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

CONFIG(debug, debug|release) {
TARGET = xmlrpcutilsd
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
}
else {
TARGET = xmlrpcutils
OBJECTS_DIR = build/release
MOC_DIR = build/release
}

include(utils.pri)




