#
# Project : QtXMLRPC
#


TEMPLATE = lib
QT = core network xml

VER_MAJ = 0
VER_MIN = 5
VER_PAT = 0


contains( CONFIG, staticlib ) {
DEFINES += QXMLRPC_STATIC
}
else {
CONFIG += dll
DEFINES += QXMLRPC_SHARED
}

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

INCLUDEPATH += client server utils .

HEADERS += \
    client/clientprotocol.h \
    client/httpclient.h \
    client/xmlrpcclient.h \
    server/xmlrpcserver.h \
    utils/xmlrpcconv.h \
    server/protocol.h \
    server/httpserver.h \
    server/deferredresult.h \
    utils/sslparams.h \
    utils/httpheader.h \
    qtxmlrpc.h \
    qtxmlrpcconfig.h \
    utils/logger.h

SOURCES += \
    client/clientprotocol.cpp \
    client/httpclient.cpp \
    client/xmlrpcclient.cpp \
    server/xmlrpcserver.cpp \
    utils/httpheader.cpp \
    utils/xmlrpcconv.cpp \
    server/protocol.cpp \
    server/httpserver.cpp \
    server/deferredresult.cpp \
    utils/sslparams.cpp


CONFIG(debug, debug|release) {
DEFINES += DEBUG_PROTOCOL DEBUG_XMLRPC
}



