/*******************************************************************************
*  file    : httpserver.cpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#include "httpserver.h"
#include <QTcpSocket>
#include <QSslSocket>
#include "utils/xmlrpcconv.h"

HttpServer::HttpServer(QAbstractSocket *parent , const int _timeout) :
    Protocol( parent, _timeout ),
    state( ReadingHeader )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "HttpServer():" << parent;
    #endif
    connect( socket, SIGNAL( readyRead()), this, SLOT( slotReadyRead()) );
    connect( socket, SIGNAL( bytesWritten( qint64)), this, SLOT( slotBytesWritten( qint64)) );
    #ifndef QT_NO_OPENSSL
    if ( socket->inherits( "QSslSocket") )
        {
            QSslSocket  *sslServer= qobject_cast < QSslSocket * > ( socket );
            sslServer->startServerEncryption();
        }
    else
        {
            if ( socket->bytesAvailable() > 0 )
                {
                    slotReadyRead();
                }
        }

    #else
    if ( socket->bytesAvailable() > 0 )
        {
            slotReadyRead();
        }
    #endif
}

void HttpServer::slotReadyRead()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotReadyRead():" << socket->bytesAvailable() << "bytes available";
    #endif
    if ( !socket->bytesAvailable() )
        return;
    switch ( state )
        {
        case ReadingHeader:
            /* если заголовок прочитан */
            if ( readRequestHeader() )
                {
                    /*
                     * если судя по заголовку есть тело, меняем
                     * статус на чтение тела ;
                     * и попадаем в следующий case
                     */
                    if ( requestContainsBody() )
                        state = ReadingBody;
                }
            else
                {
                    /*
                    * тела нет, бросаем сигнал, что заголовок
                    * получен
                    */
                    Q_ASSERT( !socket->bytesAvailable() );
                    state= WaitingReply;
                    #ifdef DEBUG_XMLRPC
                    qDebug() << this << "slotReadyRead(): emit requestReceived()";
                    #endif

                    emit    requestReceived( this, requestHeader, requestBody );
                    break;
                }
        case ReadingBody:
            if ( readRequestBody() )
                {
                    /*
                    * тело прочитано, бросаем сигнал, что запрос получен
                    */
                    Q_ASSERT( !socket->bytesAvailable() );
                    state= WaitingReply;
                    #ifdef DEBUG_XMLRPC
                    qDebug() << this << "slotReadyRead(): emit requestReceived()";
                    #endif

                    emit    requestReceived( this, requestHeader, requestBody );
                }
            break;
        case WaitingReply:
            qCritical() << this << "slotReadyRead(): got data in WaitingReply state, emit parseError()";
            emit    parseError( this );
            break;
        case SendingReply:
            qCritical() << this << "slotReadyRead(): got data in SendingHeader state, emit parseError()";
            emit    parseError( this );
            break;
        case Done:
            qCritical() << this << "slotReadyRead(): got data in Done state, emit parseError()";
            emit    parseError( this );
            break;
        default:
            qCritical() << this << "slotReadyRead(): unknown state";
            qFatal( "programming error" );
        }
}

bool HttpServer::readRequestHeader()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "readRequestHeader()";
    #endif

    /* code from qhttp.cpp */
    bool        end = false;
    QByteArray  tmp;
    QByteArray rn( "\r\n", 2 ), n( "\n", 1 );
    while ( !end && socket->canReadLine() )
        {
            tmp = socket->readLine();
            if ( tmp == rn || tmp == n || tmp.isEmpty() )
                end = true;
            else
                requestHeaderBody.append( tmp );
        }

    if ( !end )
        {
            #ifdef DEBUG_XMLRPC
            qDebug() << this << "readRequestHeader(): waiting more data, readed" << endl << requestHeaderBody;
            #endif
            return false;
        }

    //requestHeader= QHttpRequestHeader( requestHeaderBody );
    requestHeader = HttpRequestHeader( requestHeaderBody );
    requestHeaderBody.clear();
    requestBody.clear();
    if ( requestHeader.isValid() )
        {
            #ifdef DEBUG_XMLRPC
            qDebug() << this << "readRequestHeader(): header valid" << endl << requestHeader.toString();
            #endif
            return true;
        }

    qWarning() << this << "readRequestHeader(): invalid requestHeader, emit parseError()"
               << endl << requestHeader.toString();

    emit    parseError( this );
    return false;
}

bool HttpServer::readRequestBody()
{
    Q_ASSERT( requestHeader.isValid() );

    qint64  bytesToRead= ( qint64 ) requestHeader.contentLength() - ( qint64 ) requestBody.size();
    if ( bytesToRead > socket->bytesAvailable() ) bytesToRead= socket->bytesAvailable();
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "readRequestBody(): already read" << requestBody.size() << "to read" << bytesToRead;
    #endif
    requestBody.append( socket->read( bytesToRead) );
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "readRequestBody(): already read" << requestBody.size()
        << "contentLength" << requestHeader.contentLength();
    #endif
    if ( requestBody.size() == ( int ) requestHeader.contentLength() )
        return true;
    else
        return false;
}

bool HttpServer::requestContainsBody()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "requestContainsBody()";
    #endif
    Q_ASSERT( requestHeader.isValid() );
    return requestHeader.hasContentLength() && requestHeader.hasContentLength();
}

void HttpServer::slotBytesWritten( qint64 bytes )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "slotBytesWritten():" << bytes;
    #endif
    bytesWritten += bytes;
    if ( bytesWritten == bytesToWrite )
        {
            state = Done;
            emit replySent( this );
        }
}

void HttpServer::slotSendReply( const QByteArray &body )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "sendReply():" << body;
    #endif
    Q_ASSERT( state == WaitingReply );
    state = SendingReply;

    /*
     * QByteArray body = toXmlRpcResponse( e );
     */
    QByteArray hb = xmlRpcResponseHeader( body.size());
    bytesToWrite  = hb.size() + body.size();
    bytesWritten  = 0;
    socket->write( hb );
    socket->write( body );
    socket->flush();
}

void HttpServer::slotSendReply( const QVariant &e )
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "sendReply():" << e;
    #endif
    Q_ASSERT( state == WaitingReply );
    state= SendingReply;

    QByteArray          body= toXmlRpcResponse( e );
    QByteArray          hb = xmlRpcResponseHeader( body.size());
    bytesToWrite = hb.size() + body.size();
    bytesWritten = 0;
    socket->write( hb );
    socket->write( body );
    socket->flush();
}
