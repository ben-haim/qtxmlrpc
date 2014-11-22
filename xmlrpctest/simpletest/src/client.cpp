

#include "client.h"
#include <qtxmlrpc.h>
#include <QVariantList>
#include <QCoreApplication>
#include <QTimer>

Client::Client ( const QString &address, quint16 port, QObject *parent ) :
    QObject( parent ),
    address( address ),
    port( port ),
    cur_idx_(0),
    ansvers(0)
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

    qDebug() << param;
}

void Client::onDataReady( const QVariant &response )
{
    XmlRpcClient* c = qobject_cast<XmlRpcClient*>(this->sender());
    if( c )
        c->deleteLater();

    ready    = true;
    res      = response;

    if(! test_elements_.contains(response))
        qDebug()<< Q_FUNC_INFO <<" " << response.toString() ;
    else
        ansvers++;

    QTimer::singleShot(20,this, SLOT(testNext())) ;
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
    else
        {
            if(ansvers == test_elements_.size())
                qDebug() << "We received all ansvers. Test passed.";
            else
                {
                    qDebug() << "We received " << ansvers << " ansvers. But we wait for " << test_elements_.size() ;
                    qDebug() << "Test not passed.";
                }
            QTimer::singleShot(20,qApp,SLOT(quit()) );
        }
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
    testNext();
}
