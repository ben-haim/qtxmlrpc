
#include "test_server.h"
#include <QHostAddress>
#include <QDebug>
#include <QTime>


TServer::TServer ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent ),
    srv(0)
{
    srv = new XmlRpcServer;
    if ( srv->listen(QHostAddress(address), port) )
        {
            srv->registerSlot( this, SLOT(testFunc(QVariant)) );
            srv->registerSlot( this, SLOT(echo(QVariant)));
            srv->registerSlot( this, SLOT(deferredEcho(QVariant)));
            qDebug() << QTime::currentTime().toString()
                      << " Start XML-RPC server. " << "Adress:"
                      << QHostAddress( address ).toString()
                      << " Port:" << port ;
            qDebug() << "Server build with Qt " << QT_VERSION_STR ;
        }
}

TServer::~TServer()
{
    qDebug() << "Delete XML-RPC server..." ;
    srv->deleteLater();
}

QVariant TServer::testFunc( const QVariant &param )
{
    qDebug()  << QTime::currentTime().toString()
              << " testFunc: "
              << param.toString()
              ;
    return param;
}

QVariant TServer::echo(const QVariant& e)
{
    qDebug() << QTime::currentTime().toString()
              << " echo: "
              << e.toString()
              ;
    return e;
}

DeferredResult*TServer::deferredEcho(const QVariant& e)
{
    qDebug() << QTime::currentTime().toString()
              << " deferredEcho: "
              << e.toString()
              ;
    return new DeferredEcho( e );
}

#include "moc_test_server.cpp"
