

#include "client.h"
#include <qtxmlrpc.h>
#include <QVariantList>
#include <QCoreApplication>
#include <iostream>


Client::Client ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent ),
    address( address ),
    port( port ),
    cur_idx_(0)
{
    QVariantMap m;
    m["one"]  = 1;
    m["two"]  = 2;
    m["three"]= 3;
    test_elements_
            << QString("It is a string.")
            << 1
            << 1.5
            << true
            << QVariant(m)
            << QVariant( QByteArray( "Hello world!"));
}

void Client::testFunc( const QVariant &param )
{
    ready= false;

    XmlRpcClient *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL( dataReady( QVariant)), this, SLOT( onDataReady( QVariant)) );
    connect( client, SIGNAL( error ( const QString & )), this, SLOT( onError ( const QString & )) );
    client->execute( "testFunc", QVariantList() << param );

    qDebug() << Q_FUNC_INFO << "  " << param;
}

void Client::onDataReady( const QVariant &response )
{
    XmlRpcClient* c = qobject_cast<XmlRpcClient*>(this->sender());
    if( c )
        c->deleteLater();

    qDebug() << response;
    ready    = true;
    res      = response;


    std::cout<<"Client::onDataReady " << response.toString().toStdString()<<std::endl;

    testNext();

}

void Client::onError( const QString &errTxt )
{
    XmlRpcClient* c = qobject_cast<XmlRpcClient*>(this->sender());
    if( c )
        c->deleteLater();

    qDebug() << errTxt;
    ready   = true;
    res     = QVariant( errTxt );

    testNext();
}

void Client::testNext()
{
    if(cur_idx_ < test_elements_.size())
        testFunc(test_elements_[cur_idx_++]);
}

QVariant Client::operator   ( ) ( const QVariant &param )
{
    ready= false;
    XmlRpcClient    *client = new XmlRpcClient( address, port );
    connect( client, SIGNAL(dataReady(QVariant)), this, SLOT(onDataReady(QVariant)) );
    connect( client, SIGNAL(error ( const QString & )), this, SLOT(onError ( const QString & )) );
    client->execute( "testFunc", QVariantList() << param );

    return res;
}

void Client::start()
{
    testFunc(test_elements_[cur_idx_++]);
}
