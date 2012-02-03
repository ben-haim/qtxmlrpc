

#include <iostream>
#include <xmlrpcserver.h>
#include <QHostAddress>
#include "server.h"
TServer::TServer ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent )
{
    XmlRpcServer    *srv= new XmlRpcServer;
    if ( srv->listen(QHostAddress(address), port) )
      {
        srv->registerSlot( this, SLOT(testFunc(QVariant)) );
      }
}
TServer::~TServer()
{
    std::cout << "Delete XML-RPC server..." << std::endl;
}

QVariant TServer::testFunc( const QVariant &param )
{
    return param;
}
