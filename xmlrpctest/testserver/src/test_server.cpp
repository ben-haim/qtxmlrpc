
#include <iostream>
#include "test_server.h"
#include <QHostAddress>
#include <QTime>

TServer::TServer( const QString &address, quint16 port, QObject *parent )
      : QObject( parent )
{
    srv = new XmlRpcServer;
    if ( srv->listen( QHostAddress( address ), port ) )
        {
            srv->registerSlot( this, SLOT( testFunc(QVariant) ) );
        }
}

TServer::~TServer()
{
    std::cout<<"Delete XML-RPC server..."<<std::endl;
}


QVariant TServer::testFunc( const QVariant &param )
{
    std::cout<<QTime::currentTime().toString().toStdString()<<" testFunc: "<<param.toString().toStdString()<<std::endl;
    return param;
}

#include "moc_test_server.cpp"
