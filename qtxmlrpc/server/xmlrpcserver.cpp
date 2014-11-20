

#include <QDomDocument>
#include <QMetaMethod>
#include <QTimerEvent>
#include <QStringList>
#include <QDateTime>
#include <QSslSocket>
#include <QFile>
#include "xmlrpcserver.h"
#include "xmlrpcconv.h"
#include "httpheader.h"
#ifndef QT_NO_OPENSSL
#include "sslparams.h"
#endif


XmlRpcServer::XmlRpcServer( QObject *parent, const QString &cert, const QString &key, const QByteArray & ) :
    QTcpServer( parent )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "XmlRpcServer(): parent" << parent;
    #endif
    #ifndef QT_NO_OPENSSL
    if ( !cert.isEmpty() && !key.isEmpty() )
        sslParams = new SslParams( cert, key );
    else
        sslParams = NULL;
    #endif

    connect(this, SIGNAL(newConnection()), this, SLOT(onNewConnection()) );
}

XmlRpcServer::~XmlRpcServer()
{
#ifndef QT_NO_OPENSSL
    if(sslParams != NULL)
        delete sslParams;
#endif
}

void XmlRpcServer::incomingConnection(qintptr socketDescriptor )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << Q_FUNC_INFO << " " << this << "  new incoming connection";
    #endif

    QAbstractSocket *s = NULL;
    #ifndef QT_NO_OPENSSL
    if ( sslParams != NULL && !sslParams->certificate.isNull() )
        {
            s = new QSslSocket( this );
            s->setSocketDescriptor( socketDescriptor );
            ( ( QSslSocket * ) s )->setLocalCertificate( sslParams->certificate );
            ( ( QSslSocket * ) s )->setPrivateKey( sslParams->privateKey );
            ( ( QSslSocket * ) s )->setCaCertificates( sslParams->ca );
        }
    else
        {
            s = new QTcpSocket( this );
            s->setSocketDescriptor( socketDescriptor );
        }

    #else
    s = new QTcpSocket( this );
    s->setSocketDescriptor( socketDescriptor );
    #endif
    Q_ASSERT( s->state() == QAbstractSocket::ConnectedState );
    connect( s, SIGNAL( disconnected()), this, SLOT( slotSocketDisconnected()) );

    HttpServer * p = new HttpServer( s );
    connect( p, SIGNAL( protocolTimeout ( Protocol * )), this, SLOT( slotProtocolTimeout ( Protocol * )) );
    connect( p, SIGNAL( parseError ( HttpServer * )), this, SLOT( slotParseError ( HttpServer * )) );
    connect( p, SIGNAL( requestReceived( HttpServer *, const HttpRequestHeader &, const QByteArray &)), this,
             SLOT( slotRequestReceived( HttpServer *, const HttpRequestHeader &, const QByteArray &)) );
    connect( p, SIGNAL( replySent ( HttpServer * )), this, SLOT( slotReplySent ( HttpServer * )) );
}

void XmlRpcServer::slotSocketDisconnected()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotSocketDisconnected()";
    #endif

    /* just delete this socket. */
    sender()->deleteLater();
}

void XmlRpcServer::slotProtocolTimeout( Protocol *p )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotProtocolTimeout()";
    #endif
    qWarning() << this << "slotProtocolTimeout(): client"
               << p->getSocket()->peerAddress() << ":"
               << p->getSocket()->peerPort() << "closing connection";
    p->getSocket()->close();
}

void XmlRpcServer::slotParseError( HttpServer *p )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotParseError()";
    #endif
    qWarning() << this << "slotParseError(): client"
               << p->getSocket()->peerAddress() << ":"
               << p->getSocket()->peerPort()
               << "closing connection";
    p->getSocket()->close();
}

void XmlRpcServer::registerSlot( QObject *receiver, const char *slot, QByteArray path )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "registerSlot():" << receiver << slot;
    #endif

    /* skip code first symbol 1 from SLOT macro (from qobject.cpp) */
    ++slot;

    /* find QMetaMethod.. */
    const QMetaObject   *mo= receiver->metaObject();

    /* we need buf in memory for const char (from qobject.cpp) */
    QByteArray          method= QMetaObject::normalizedSignature( slot );
    const char          *normalized= method.constData();
    int                 slotId= mo->indexOfSlot( normalized );
    if ( slotId == -1 )
        {
            qCritical() << this << "registerSlot():" << receiver << normalized << "can't find slot";
            qFatal( "programming error" );
        }

    QMetaMethod m= receiver->metaObject()->method( slotId );
    bool        isDefferedResult= !qstrcmp( m.typeName(), "DeferredResult*" );
    if ( qstrcmp( m.typeName(), "QVariant") && !isDefferedResult )
        {
            qCritical() << this << "registerSlot():" << receiver << normalized
                        << "rpc return type should be QVariant or DeferredResult*, but"
                        << m.typeName();
            qFatal( "programming error" );
        }

    foreach( QByteArray c, m.parameterTypes() ) if ( c != "QVariant" )
        {
            qCritical() << this << "registerSlot():" << receiver << normalized
                        << "all parameters should be QVariant";
            qFatal( "programming error" );
        }

    /* ok, now lets make just function name from our SLOT */
    Q_ASSERT( method.indexOf( '(') > 0 );
    method.truncate( method.indexOf( '(') );
    if ( path[0] != '/' )
        path.prepend( '/' );
    if ( path[path.size() - 1] != '/' )
        path.append( '/' );
    method.prepend( path );

    /* check if allready exists */
    if ( callbacks.contains( method) )
        {
            qCritical() << this << "registerSlot():" << receiver << method
                        << "allready registered, receiver" << callbacks[method];
            qFatal( "programming error" );
        }

    callbacks[method]= IsMethodDeffered( receiver, isDefferedResult );

    Methods &methods= objectMethods[receiver];
    if ( methods.isEmpty() )
        {
            #ifdef DEBUG_XMLRPC
            qDebug() << this << "registerSlot(): connecting SIGNAL(destroyed()) of" << receiver;
            #endif
            connect( receiver, SIGNAL( destroyed ( QObject * )), this, SLOT( slotReceiverDestroed ( QObject * )) );
        }

    methods.append( method );
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "registerSlot(): callbacks" << callbacks;
    qDebug() << this << "registerSlot(): objectMethods" << objectMethods;
    #endif
}

DeferredResult*XmlRpcServer::deferredEcho(const QVariant& e)
{
    return new DeferredEcho( e );
}

void XmlRpcServer::slotReceiverDestroed( QObject *o )
{
#ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReceiverDestroed():" << o;
    #endif
    Q_ASSERT( objectMethods.contains( o) );
    foreach ( const char *m, objectMethods[o] ) if ( !callbacks.remove( m) )
        qCritical() << this << "slotReceiverDestroed():" << "can't remove" << m << "from callbacks";
    objectMethods.remove( o );
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReceiverDestroed(): callbacks" << callbacks;
    qDebug() << this << "slotReceiverDestroed(): objectMethods" << objectMethods;
#endif
}

void XmlRpcServer::onNewConnection()
{
#ifdef DEBUG_XMLRPC
qDebug() << this << Q_FUNC_INFO;
#endif
}

void XmlRpcServer::slotRequestReceived(HttpServer *p,
                                       const HttpRequestHeader& h,
                                       const QByteArray &body )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotRequestReceived():" << p << h.toString() << body;
    #endif

    QDomDocument    doc;
    QString         errorMsg;
    int             errorLine, errorColumn;
    if ( !doc.setContent( body, &errorMsg, &errorLine, &errorColumn) )
        {
            qCritical() << this << "slotRequestReceived(): can't parse xml"
                        << endl << body << endl << errorMsg << endl
                        << "line:" << errorLine << "column:" << errorColumn;
            p->getSocket()->close();
            return;
        }

    QVariantList    params;
    QDomNode        n;
    QDomElement     e;

    /* run until first element */
    n= doc.firstChild();
    while ( !n.isNull() )
        {
            e= n.toElement();
            if ( !e.isNull() )
                break;
            n= n.nextSibling();
        }

    if ( e.tagName() != "methodCall" )
        {
            qCritical() << this << "slotRequestReceived(): first element is not methodCall"
                        << e.tagName() << endl << body;
            p->getSocket()->close();
            return;
        }

    /* first child SHOULD be methodName */
    e= e.firstChild().toElement();
    if ( e.tagName() != "methodName" )
        {
            qCritical() << this << "slotRequestReceived(): first element of methodCall is not methodName"
                        << e.tagName()
                        << endl << body;
            p->getSocket()->close();
            return;
        }

    QByteArray  methodName= e.firstChild().toText().data().toLatin1();
    if ( methodName.isEmpty() )
        {
            qCritical() << this << "slotRequestReceived(): methodName empty" << endl << body;
            p->getSocket()->close();
            return;
        }

    QByteArray  path= h.path().toLatin1();
    if ( path[0] != '/' )
        path.prepend( '/' );
    if ( path[path.size() - 1] != '/' )
        path.append( '/' );
    if ( !callbacks.contains( path + methodName) )
        {
            qCritical() << this << "slotRequestReceived(): method" << path + methodName
                        << "is not registered as callback";
            p->slotSendReply( toXmlRpcResponse( createFault( -1, "method " + path + methodName + " not registered")) );
            return;
        }

    /* run over params */
    e= e.nextSibling().toElement();
    if ( e.tagName() != "params" )
        {
            qCritical() << this << "slotRequestReceived(): method"
                        << methodName << "no params element in xml request" << endl
                        << body;
            p->slotSendReply( toXmlRpcResponse( createFault( -1, "no params element in xml request")) );
            return;
        }

    /* run until first element */
    e= e.firstChild().toElement();
    while ( !e.isNull() )
        {
            if ( e.tagName() != "param" )
                {
                    qCritical() << this << "slotRequestReceived(): method"
                                << methodName << "no param element in xml request"
                                << endl << body;
                    p->slotSendReply( toXmlRpcResponse( createFault( -1, "bad param element in xml request")) );
                    return;
                }

            QString err;
            params.append( parseXmlRpcValue( e.firstChild().toElement(), err) );
            if ( !err.isEmpty() )
                {
                    qCritical() << this << "slotRequestReceived(): method"
                                << methodName << "parse error:" << err << endl << body;
                    p->slotSendReply( toXmlRpcResponse( createFault( -1, err)) );
                    return;
                }

            e= e.nextSibling().toElement();
        }

    QAbstractSocket *s= p->getSocket();
    #ifndef QT_NO_OPENSSL
    if ( s->inherits( "QSslSocket") )
        {
            QSslSocket  *sslSkt= qobject_cast < QSslSocket * > ( s );
            #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
            QString     commonName = sslSkt->peerCertificate().subjectInfo( QSslCertificate::CommonName );
            #else
            QString     commonName = sslSkt->peerCertificate()
                                            .subjectInfo( QSslCertificate::CommonName ).join(",");
            #endif
            params.append( commonName );
        }
    #endif

    /* params parsed, now invoke registered slot. */
    QVariant                retVal;
    DeferredResult          *retDeffered;
    const IsMethodDeffered  &md= callbacks[path + methodName];
    bool                    invoked;
    if ( md.second )
        {
            invoked = QMetaObject::invokeMethod( md.first, methodName.constData(), Qt::DirectConnection,
                                                Q_RETURN_ARG( DeferredResult *, retDeffered),
                                                params.size() > 0 ? Q_ARG( QVariant, params[0]) : QGenericArgument(),
                                                params.size() > 1 ? Q_ARG( QVariant, params[1]) : QGenericArgument(),
                                                params.size() > 2 ? Q_ARG( QVariant, params[2]) : QGenericArgument(),
                                                params.size() > 3 ? Q_ARG( QVariant, params[3]) : QGenericArgument(),
                                                params.size() > 4 ? Q_ARG( QVariant, params[4]) : QGenericArgument(),
                                                params.size() > 5 ? Q_ARG( QVariant, params[5]) : QGenericArgument(),
                                                params.size() > 6 ? Q_ARG( QVariant, params[6]) : QGenericArgument(),
                                                params.size() > 7 ? Q_ARG( QVariant, params[7]) : QGenericArgument(),
                                                params.size() > 8 ? Q_ARG( QVariant, params[8]) : QGenericArgument(),
                                                params.size() > 9 ? Q_ARG( QVariant, params[9]) : QGenericArgument() );
        }
    else
        {
            invoked = QMetaObject::invokeMethod( md.first, methodName.constData(), Qt::DirectConnection,
                                                Q_RETURN_ARG( QVariant, retVal),
                                                params.size() > 0 ? Q_ARG( QVariant, params[0]) : QGenericArgument(),
                                                params.size() > 1 ? Q_ARG( QVariant, params[1]) : QGenericArgument(),
                                                params.size() > 2 ? Q_ARG( QVariant, params[2]) : QGenericArgument(),
                                                params.size() > 3 ? Q_ARG( QVariant, params[3]) : QGenericArgument(),
                                                params.size() > 4 ? Q_ARG( QVariant, params[4]) : QGenericArgument(),
                                                params.size() > 5 ? Q_ARG( QVariant, params[5]) : QGenericArgument(),
                                                params.size() > 6 ? Q_ARG( QVariant, params[6]) : QGenericArgument(),
                                                params.size() > 7 ? Q_ARG( QVariant, params[7]) : QGenericArgument(),
                                                params.size() > 8 ? Q_ARG( QVariant, params[8]) : QGenericArgument(),
                                                params.size() > 9 ? Q_ARG( QVariant, params[9]) : QGenericArgument() );
        }

    if ( !invoked )
        {
            qCritical() << this << "slotRequestReceived(): can't invoke" << methodName << params;

            QVariant    fault = createFault( -1, "can't invoke " + path + methodName + ", wrong parameters number?" );
            p->slotSendReply( toXmlRpcResponse( fault) );
            return;
        }

    if ( md.second )
        {
            retDeffered->setParent( p->getSocket() );
            connect( retDeffered, SIGNAL( sendReply ( const QVariant & )), p, SLOT( slotSendReply ( const QVariant & )) );
        }
    else
        p->slotSendReply( toXmlRpcResponse( retVal) );
    return;
}

void XmlRpcServer::slotReplySent( HttpServer *p )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReplySent():" << p;
    #endif
    p->getSocket()->close();
}

