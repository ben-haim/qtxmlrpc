

#ifndef XMLRPCCLIENT_H
#define XMLRPCCLIENT_H
#include "httpclient.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QVariantList>

class XmlRpcClient :
    public HttpClient
{
    Q_OBJECT

public:
    XmlRpcClient( const QString &host, const quint16 port, QObject* parent = 0 );
    XmlRpcClient( const QString &host, const quint16 port, const QString &path, QObject* parent = 0 );
    void     execute( const QString &method, const QVariantList &params );
    bool     isReady() const;
    QVariant response() const;
    QString  errString() const;
signals:
    void     dataReady( const QVariant &data );
private slots :
    void     onDataReady ( const QByteArray &data );
    void     onError(const QString & errStr);
private:
    bool     isReady_;
    QString  err_;
    QVariant response_;
};
#endif /* XMLRPCCLIENT_H */
