

#include "xmlrpcclient.h"
#include "xmlrpcconv.h"
XmlRpcClient::XmlRpcClient ( const QString &host, const quint16 port ) :
    HttpClient( host, port, "/RPC2", HttpClient::POST ), isReady_(false)
{

}

void XmlRpcClient::execute( const QString &method, const QVariantList &params )
{
    isReady_ = false;
    setPostData( toXmlRpcRequest( method, params) );
    connect( this, SIGNAL( dataReady( QByteArray)), SLOT( onDataReady( QByteArray)), Qt::UniqueConnection );
    deferredStart();
}

bool XmlRpcClient::isReady() const
{
    return isReady_;
}

void XmlRpcClient::onDataReady( const QByteArray &data )
{
    QString     err;
    QVariant    response= fromXmlRpcResponse( data, err );

    /* TODO fault */
    emit        dataReady( response );
    isReady_ = true;
}
