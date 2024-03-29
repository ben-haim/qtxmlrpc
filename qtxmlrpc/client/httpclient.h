

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "clientprotocol.h"
#include <QUrl>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <utils/httpheader.h>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include "qtxmlrpcconfig.h"

// #define HTTP_CLIENT_DEBUG ;
// #define TRACE_HTTP

class QXMLRPC_DECL HttpClient : public NetworkClient
{
    Q_OBJECT
public:
    enum HttpMethod { GET, POST };
    HttpClient( const QString &host,
                const quint16 port= 80,
                const QString &path= "/",
                const HttpMethod method = GET,
                QObject* parent = 0);
    ~HttpClient();
    inline void setPostData( const QByteArray &ba )     { postData = ba; }
    inline void setReferer( const QString &value )      { referer = value; }
signals:
    void    dataRecieved();
    void    dataReady( const QByteArray &data );
protected slots :
    void    onReadyRead();
    void    onProtocolDone();

protected:
    void    protocolStop();
    void    protocolStart();

private:
    enum HttpState { Waiting, Sending, ReadingResponseHeader, ReadingResponseBody } httpState;
    bool readResponseHeader();
    bool readResponseBody();
    bool readChunked();
    bool readContentLength();
private:
    QUrl               url;
    HttpMethod         method;
    QByteArray         postData;
    QString            referer;
    QNetworkCookieJar  cookieJar;
    HttpResponseHeader responseHeader;
    QByteArray         responseHeaderData;
    QByteArray         responseBodyData;
    qint64             chunkedSize;
    static const int   requestTimeout= 5000; /* msecs */
};
#endif /* HTTPCLIENT_H */
