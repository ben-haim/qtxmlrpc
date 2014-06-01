/*******************************************************************************
*  file    : protocol.hpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <QObject>

class QAbstractSocket;

/*
 =======================================================================================================================
    Protocol is base class for Network protocols. This implementation implement timeout, if not data sent/received.
    Protocol shoud be created as QAbstractSocket child, if you delete socket, protocol will be deleted automaticly
    Socket can have only one protocol at one time. Protocol have no start/stop policy, it start when construct and
    stops when destruct.
 =======================================================================================================================
 */
class Protocol : public QObject
{
    Q_OBJECT
public:

    /* Timeout activates, if it is above zero, msecs. */
    Protocol( QAbstractSocket *parent, const int _timeout= 0 );
    ~Protocol();
    void            setTimeout( const int _timeout );
    void            stopProtocolTimeout()   { setTimeout( 0 ); }
    QAbstractSocket *getSocket()            { return socket; }
    private slots :

    /* restarts timeout */
    void __slotReadyRead();
    void __slotBytesWritten( qint64 bytes );
signals:
    void    protocolTimeout( Protocol * );

protected:
    void            timerEvent( QTimerEvent *event );
    void            restartProtocolTimeoutTimer();
    int             timeout;    /* таймаут протокола. */
    int             timeoutTimerId;

    /* Just not to cast parent() every time you need socket */
    QAbstractSocket *socket;
};

#endif // PROTOCOL_HPP
