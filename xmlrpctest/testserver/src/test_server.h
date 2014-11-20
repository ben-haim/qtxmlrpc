
#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QVariant>
#include <qtxmlrpc.h>

class TServer :
    QObject
{
    Q_OBJECT
public:
    TServer( const QString &address, quint16 port, QObject *parent= 0 );
    ~   TServer();
private slots :
    QVariant testFunc ( const QVariant &param );
    QVariant echo( const QVariant& e );
    DeferredResult * deferredEcho( const QVariant& e );

private:
    XmlRpcServer    *srv;
};
#endif /* SERVER_H */
