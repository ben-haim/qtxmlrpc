#
# Qxmlrpc Client
#
TEMPLATE = lib
CONFIG += staticlib
#CONFIG += dll
QT -= gui

#DEFINES += QT_NO_OPENSSL

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
}

#TODO: here in future replace
contains( CONFIG, dll ) {
    win32:DEFINES += QLOGGER_SHARED
}

include(../utils/utils.pri)
include(client.pri)


CONFIG(debug, debug|release) {
    DESTDIR = ./lib
    TARGET = xmlrpcclientd
} else {
    DESTDIR = ./lib
    TARGET = xmlrpcclient
}


