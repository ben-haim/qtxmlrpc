#
# Project : QtXMLRPC
#


TEMPLATE = lib
CONFIG += staticlib
QT = core network xml

DESTDIR = ../lib/$$basename(QMAKESPEC)/$$QT_VERSION

UI_HEADERS_DIR = build
UI_DIR         = build

CONFIG(debug, debug|release) {
OBJECTS_DIR = build/debug
MOC_DIR = build/debug
TARGET = qtxmlrpcd
CONFIG += console
}
else {
OBJECTS_DIR = build/release
MOC_DIR = build/release
TARGET = qtxmlrpc
}

INCLUDEPATH += client server utils

HEADERS += \
    client/clientprotocol.h \
    client/httpclient.h \
    client/xmlrpcclient.h \
    server/xmlrpcserver.h \
    utils/httpheader.hpp \
    utils/xmlrpcconv.h

SOURCES += \
    client/clientprotocol.cpp \
    client/httpclient.cpp \
    client/xmlrpcclient.cpp \
    server/xmlrpcserver.cpp \
    utils/httpheader.cpp \
    utils/xmlrpcconv.cpp






