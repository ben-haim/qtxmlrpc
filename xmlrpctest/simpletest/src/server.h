#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QVariant>

class XmlRpcServer;

class TServer : public QObject
{
    Q_OBJECT

public:
    TServer( const QString &address, quint16 port, QObject *parent= 0 );
    ~TServer();
private slots :
    QVariant testFunc ( const QVariant &param );

private:
    XmlRpcServer    *srv_;
};

#endif /* SERVER_H */
