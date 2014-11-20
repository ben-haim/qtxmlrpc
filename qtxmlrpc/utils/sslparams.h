/*******************************************************************************
*  file    : sslparams.h
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#ifndef SSLPARAMS_H
#define SSLPARAMS_H

/* ssl */
#include <QSslCertificate>
#include <QSslKey>

#ifndef QT_NO_OPENSSL
class SslParams
{
public:
    SslParams( const QString &certFile, const QString &keyFile);
    QSslCertificate         certificate;
    QList<QSslCertificate>  ca;
    QSslKey                 privateKey;
};
#endif /* QT_NO_OPENSSL */

#endif // SSLPARAMS_H
