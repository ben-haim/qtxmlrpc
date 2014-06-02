/*******************************************************************************
*  file    : httpserver.hpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "protocol.h"
#include "utils/httpheader.h"

/* very basic HttpServer for XmlRpc */
class HttpServer : public Protocol
{
    Q_OBJECT
public:
    HttpServer( QAbstractSocket *parent, const int _timeout = 10000  );
public slots :

    /* sends xmlrpc response from QVariant param */
    void    slotSendReply ( const QByteArray & );
    void    slotSendReply ( const QVariant & );
protected slots :
    void    slotReadyRead();
    void    slotBytesWritten( qint64 bytes );
signals:
    void    parseError( HttpServer * );
    void    requestReceived( HttpServer *,
                             const HttpRequestHeader &h,
                             const QByteArray &body );
    void    replySent( HttpServer * );

private:
    bool    readRequestHeader();
    bool    readRequestBody();
    bool    requestContainsBody();
    enum State { ReadingHeader, ReadingBody, WaitingReply, SendingReply, Done } state;
    QString requestHeaderBody;
    QByteArray requestBody;
    HttpRequestHeader requestHeader;
    qint64 bytesWritten;
    qint64 bytesToWrite;
};

#endif // HTTPSERVER_HPP
