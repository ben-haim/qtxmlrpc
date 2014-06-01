/*******************************************************************************
*  file    : deferredresult.cpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#include "deferredresult.h"
#include <QDebug>

DeferredEcho::DeferredEcho( const QVariant &e ) :
    echo( e )
{
    startTimer( 1000 ); /* one second timeout, before echo */
}

void DeferredEcho::timerEvent( QTimerEvent * )
{
    emit    sendReply( echo );
}

DeferredResult::DeferredResult()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "DeferredResult()";
    #endif
}

DeferredResult::~DeferredResult()
{
    #ifdef DEBUG_XMLRPC
    qDebug() << this << "~DeferredResult()";
    #endif
}
