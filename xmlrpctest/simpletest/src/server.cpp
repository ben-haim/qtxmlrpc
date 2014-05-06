

#include <iostream>
#include <qtxmlrpc.hpp>
#include <QHostAddress>
#include "server.h"


TServer::TServer ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent ),
    srv_(0)
{
    srv_ = new XmlRpcServer;
    if ( srv_->listen(QHostAddress(address), port) )
        {
            srv_->registerSlot( this, SLOT(testFunc(QVariant)) );
            qDebug() << "Server start listening.";
        }
    else
        {
            qDebug() << "Error: Can't start server.";
        }
}
TServer::~TServer()
{
    std::cout << "Delete XML-RPC server..." << std::endl;
    delete srv_;
}

QVariant TServer::testFunc( const QVariant &param )
{
    qDebug() << Q_FUNC_INFO << " " << param;
    return param;
}
