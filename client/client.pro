#
# Qxmlrpc Client
#
TEMPLATE = lib
CONFIG += staticlib
QT = core network xml

DESTDIR = ../lib/$$basename(QMAKESPEC)/$$QT_VERSION

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



