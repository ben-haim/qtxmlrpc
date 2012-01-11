
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
            std::cout<<QTime::currentTime().toString().toStdString()<<" Start XML-RPC server. "
                    <<"Adress:"<<QHostAddress( address ).toString().toStdString()
                   <<" Port:"<<port
                   <<std::endl;
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
