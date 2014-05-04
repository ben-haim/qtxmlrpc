

#include "xmlrpcclient.h"
#include "xmlrpcconv.h"


XmlRpcClient::XmlRpcClient (const QString &host, const quint16 port , QObject* parent) :
    HttpClient( host, port, "/RPC2", HttpClient::POST, parent ),
    isReady_(false)
{
    connect( this, SIGNAL( dataReady( QByteArray)), SLOT( onDataReady( QByteArray)), Qt::UniqueConnection );
    connect( this, SIGNAL(error(QString)), SLOT(onError(QString)), Qt::UniqueConnection);
}

XmlRpcClient::XmlRpcClient(const QString& host, const quint16 port, const QString& path, QObject* parent) :
    HttpClient( host, port, path, HttpClient::POST, parent ),
    isReady_(false)
{
    connect( this, SIGNAL( dataReady( QByteArray)), SLOT( onDataReady( QByteArray)), Qt::UniqueConnection );
    connect( this, SIGNAL(error(QString)), SLOT(onError(QString)), Qt::UniqueConnection);
}

void XmlRpcClient::execute( const QString &method, const QVariantList &params )
{
    isReady_ = false;
    err_ = "";
    response_ = QVariant();
    setPostData( toXmlRpcRequest( method, params) );
    deferredStart();
}

bool XmlRpcClient::isReady() const
{
    return isReady_;
}

QVariant XmlRpcClient::response() const
{
    return response_;
}

QString XmlRpcClient::errString() const
{
    return err_;
}

void XmlRpcClient::onDataReady( const QByteArray &data )
{
    response_ = fromXmlRpcResponse( data, err_ );

    /* TODO fault */
    emit        dataReady( response_ );
    isReady_ = true;
}

void XmlRpcClient::onError(const QString &errStr)
{
    response_ = QVariant();
    err_ = errStr;
    emit        dataReady( response_ );
    isReady_ = true;
}


#include "moc_xmlrpcclient.cpp"
