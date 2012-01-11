#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QVariant>
#include <xmlrpcserver.h>


class TServer : QObject
{
      Q_OBJECT

public:
      TServer( const QString &address, quint16 port, QObject *parent = 0 );
      ~TServer();

private slots:
      QVariant testFunc( const QVariant &param );
private:
      XmlRpcServer *srv;
};

#endif // SERVER_H
