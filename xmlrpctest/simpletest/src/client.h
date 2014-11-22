#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QVariant>

class Client : public QObject
{
    Q_OBJECT

public:
    Client( const QString &address, quint16 port, QObject *parent= 0 );
    void     testFunc( const QVariant &param );
    QVariant operator () ( const QVariant &param );
public slots:
    void start();
private slots :
    void    onDataReady ( const QVariant &response );
    void    onError( const QString &errTxt );
    void    testNext();
private:
    QString     address;
    quint16     port;
    bool        ready;
    QVariant    res;

    QVariantList test_elements_;
    int          cur_idx_;
    int          ansvers;
};

#endif /* CLIENT_H */
