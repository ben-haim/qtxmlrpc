/*******************************************************************************
*  file    : protocol.cpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#include "protocol.h"
#include <QDebug>
#include <QAbstractSocket>
#include <QTimerEvent>

Protocol::Protocol( QAbstractSocket *parent, const int _timeout ) :
    QObject( parent ),
    timeout( 0 ),
    timeoutTimerId( 0 ),
    socket( parent )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "Protocol(): child of" << socket;
    #endif

    /* check we are only one protocol, working with this socket, at this time */
    foreach( QObject * o, parent->children() )
        if ( o != this && qobject_cast < Protocol * > (o) )
            {
                qCritical() << this << "Protocol(): socket" << parent << "already have another protocol" << o;
                qFatal( "programming error" );
            }

    if ( _timeout > 0 ) setTimeout( _timeout );
}

Protocol::~Protocol()
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "~Protocol()";
    #endif
}

/*
 =======================================================================================================================
    timeout restarts
 =======================================================================================================================
 */
void Protocol::setTimeout( const int _timeout )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "setTimeout():" << _timeout << "prev timeout was" << timeout;
    #endif
    if ( timeoutTimerId )
        {
            #ifdef DEBUG_PROTOCOL
            qDebug() << this << "setTimeout(): stopping previous timer";
            #endif
            killTimer( timeoutTimerId );
            timeoutTimerId = 0;
        }

    if ( !(timeout > 0) && _timeout > 0 )
        {
            /*
            * если предыдущий timeout НЕ был больше ноля, а текущий больше,
            * значит нужно прицепить сигналы приема/отсылки данных на сокете
            */
            #ifdef DEBUG_PROTOCOL
            qDebug() << this << "setTimeout(): connect socket signals readyRead() and bytesWritten()";
            #endif
            connect( socket, SIGNAL( readyRead()), this, SLOT( __slotReadyRead()) );
            connect( socket, SIGNAL( bytesWritten( qint64)), this, SLOT( __slotBytesWritten( qint64)) );
        }
    else if ( timeout > 0 && !(_timeout > 0) )
        {
            /* новый выключен, старый был включен */
            #ifdef DEBUG_PROTOCOL
            qDebug() << this << "setTimeout(): disconnect socket signals readyRead() and bytesWritten()";
            #endif
            disconnect( socket, SIGNAL( readyRead()), this, SLOT( __slotReadyRead()) );
            disconnect( socket, SIGNAL( bytesWritten( qint64)), this, SLOT( __slotBytesWritten( qint64)) );
        }

    timeout= _timeout;
    if ( timeout > 0 )
        restartProtocolTimeoutTimer();
}

void Protocol::restartProtocolTimeoutTimer()
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "restartProtocolTimeoutTimer()";
    #endif
    if ( timeoutTimerId )
        killTimer( timeoutTimerId );
    Q_ASSERT( timeout > 0 );
    timeoutTimerId= startTimer( timeout );
}

void Protocol::timerEvent( QTimerEvent *event )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "timerEvent():" << event->timerId() << "protocol timeout timer id" << timeoutTimerId;
    #endif
    if ( event->timerId() == timeoutTimerId )
        {
            #ifdef DEBUG_PROTOCOL
            qDebug() << this << "timerEvent(): emit ProtocolTimeout()";
            #endif

            emit    protocolTimeout( this );
            killTimer( timeoutTimerId );
            timeoutTimerId = 0;
        }
}

void Protocol::__slotReadyRead()
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "__slotReadyRead():" << socket->bytesAvailable()
             << "bytes available, restarting protocol timeout timer";
    #endif
    restartProtocolTimeoutTimer();
}

void Protocol::__slotBytesWritten( qint64  bytes  )
{
    #ifdef DEBUG_PROTOCOL
    qDebug() << this << "__slotBytesWritten():" << bytes << "written, restarting protocol timeout timer";
    #else
    Q_UNUSED(bytes)
    #endif
    restartProtocolTimeoutTimer();
}
