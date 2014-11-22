
#include <qtxmlrpc.h>
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
    qDebug() << "Delete XML-RPC server..." ;
    delete srv_;
}

QVariant TServer::testFunc( const QVariant &param )
{
    return param;
}
