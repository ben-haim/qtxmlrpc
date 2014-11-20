/*******************************************************************************
*  file    : sslparams.cpp
*  created : 02.06.2014
*  author  :
*******************************************************************************/

#include "sslparams.h"
#include <QFile>

#ifndef QT_NO_OPENSSL
SslParams::SslParams ( const QString &certFile, const QString &keyFile)
{
    QFile   fc( certFile );
    fc.open( QFile::ReadOnly );
    certificate = QSslCertificate( fc.readAll() );
    fc.close();
    ca << certificate;
    QFile   fk( keyFile );
    fk.open( QFile::ReadOnly );
    privateKey = QSslKey( fk.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey );
    fk.close();
}
#endif
