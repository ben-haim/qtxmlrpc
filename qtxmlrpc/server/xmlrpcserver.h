

#ifndef XMLRPCSERVER_H
#define XMLRPCSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QList>
#include <QString>
#include <QVariant>
#include <utils/httpheader.h>
#include "qtxmlrpcconfig.hpp"
#include "httpserver.h"
#include "deferredresult.h"

#ifndef QT_NO_OPENSSL
#include "utils/sslparams.h"
#endif

/*
 * #define DEBUG_XMLRPC ;
 * use spaces in xmlrpc formattin
 */
#define XMLRPC_WITHSPACES

class QXMLRPC_DECL XmlRpcServer : public QTcpServer
{
    Q_OBJECT
public:
    XmlRpcServer( QObject *parent= 0, const QString &cert= "", const QString &key= "",
                  const QByteArray &passwd= QByteArray() );
    ~XmlRpcServer();

    /*
     * Slots should return QVariant and accept const QVariant& as params. ;
     * QVariant method( const QVariant &, ... );
     * path + slot should be unique.
     */
    void    registerSlot( QObject *receiver, const char *slot, QByteArray path= "/RPC2/" );
public slots :
    QVariant echo ( const QVariant &e ){return e;}
    DeferredResult  *deferredEcho( const QVariant &e );
private slots :
    /*
     * we don't support SSL yet,
     * if you need SSL, connection handling design must be revised a bit ;
     * void slotNewConnection();
     * ;
     * we don't need socket more, if it's disconnected
     */
    void slotSocketDisconnected();

    /* protocol errors */
    void    slotProtocolTimeout( Protocol * );
    void    slotParseError( HttpServer * );

    /* when HttpServer receives request */
    void    slotRequestReceived( HttpServer *,
                                 const HttpRequestHeader &h,
                                 const QByteArray &body );

    /* when reply sent */
    void    slotReplySent( HttpServer * );

    /* when registrant dies, we need to remove their callbacks методов. */
    void    slotReceiverDestroed( QObject * );

    void    onNewConnection();

protected:
    void    incomingConnection( qintptr socketDescriptor );

private:

    /* if Method is deffered */
    typedef QPair<QObject *, bool>              IsMethodDeffered;

    /* fast access for invokeMethod */
    typedef QHash<QByteArray, IsMethodDeffered> Callbacks;

    /* fast callbacks delete, when registrant dies */
    typedef QList<QByteArray>                   Methods;
    typedef QMap<QObject *, Methods>            ObjectMethods;
    Callbacks                                   callbacks;
    ObjectMethods                               objectMethods;
#ifndef QT_NO_OPENSSL
    SslParams                                   *sslParams;
#endif
};
#endif
