/*******************************************************************************
*  file    : deferredresult.hpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#ifndef DEFERREDRESULT_HPP
#define DEFERREDRESULT_HPP

#include <QObject>
#include <QVariant>

/*
 =======================================================================================================================
    you can deriver from this class and implement deferredRun ;
    After deferred sql, network and so, you can emit sendReply signal with result. ;
    This signal will be connected to your HttpServer slotSendReply. ;
    DeferredResult will be deleted, as socket child, or you can this->deleteLater(), ;
    after emmiting result.
 =======================================================================================================================
 */
class DeferredResult : public QObject
{
    Q_OBJECT
public:
    DeferredResult();
    ~DeferredResult();
signals:
    void    sendReply( const QVariant & );
    void    sendReply( const QByteArray & );
};

class DeferredEcho : public DeferredResult
{
    Q_OBJECT
public:
    DeferredEcho( const QVariant &e );

private:
    void        timerEvent( QTimerEvent * );
    QVariant    echo;
};

#endif // DEFERREDRESULT_HPP
