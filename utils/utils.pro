#
# Qxmlrpc Utils
#
TEMPLATE = lib
CONFIG += staticlib
QT = core xml network

greaterThan(QT_MAJOR_VERSION, 4){
DESTDIR = ../libq5
}
else {
DESTDIR = ../lib
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




