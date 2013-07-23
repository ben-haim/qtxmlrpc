

#include <iostream>
#include "test_server.h"
#include <QHostAddress>
#include <QTime>
TServer::TServer ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent )
{
    srv= new XmlRpcServer;
    if ( srv->listen(QHostAddress(address), port) )
      {
        srv->registerSlot( this, SLOT(testFunc(QVariant)) );
        srv->registerSlot( this, SLOT(echo(QVariant)));
        srv->registerSlot( this, SLOT(deferredEcho(QVariant)));
        std::cout << QTime::currentTime().toString().toStdString()
        << " Start XML-RPC server. " << "Adress:" <<
        QHostAddress( address ).toString().toStdString()
        << " Port:" << port << std::endl;
      }
}
TServer::~TServer()
{
    std::cout << "Delete XML-RPC server..." << std::endl;
}

QVariant TServer::testFunc( const QVariant &param )
{
    std::cout <<
        QTime::currentTime().toString().toStdString() <<
        " testFunc: " <<
        param.toString().toStdString() <<
        std::endl;
    return param;
}

QVariant TServer::echo(const QVariant& e)
{
    std::cout <<
        QTime::currentTime().toString().toStdString() <<
        " echo: " <<
        e.toString().toStdString() <<
        std::endl;
    return e;
}

DeferredResult*TServer::deferredEcho(const QVariant& e)
{
    std::cout <<
        QTime::currentTime().toString().toStdString() <<
        " deferredEcho: " <<
        e.toString().toStdString() <<
        std::endl;
    return new DeferredEcho( e );
}

#include "moc_test_server.cpp"
