#ifndef XMLRPCCONV_H
#define XMLRPCCONV_H

#include <QVariant>
#include <QByteArray>
#include <QDomElement>
#include "qtxmlrpcconfig.h"

QByteArray  QXMLRPC_DECL toXmlRpcRequest   ( const QString m, const QVariantList &l );
QVariant    QXMLRPC_DECL fromXmlRpcResponse( const QString d,       QString      &err );

/* returns default http header for our xmlrpc server */
QByteArray  QXMLRPC_DECL        xmlRpcResponseHeader( const qint64 contentLength );

/* create xmlrpc response from QVariant */
QByteArray  QXMLRPC_DECL toXmlRpcResponse( const QVariant &v );

/*
 * QVariant to xml conversions ;
 * use QByteArray & reference, becouse it is faster, then return QByteArray
 */
void        QXMLRPC_DECL toXmlRpcValue ( const int spaces, const QVariant     &child, QByteArray &b );
void        QXMLRPC_DECL toXmlRpcStruct( const int spaces, const QVariantMap  &child, QByteArray &b );
void        QXMLRPC_DECL toXmlRpcArray ( const int spaces, const QVariantList &child, QByteArray &b );

/* xml to QVariant conversions */
QVariant    QXMLRPC_DECL parseXmlRpcValue ( const QDomElement &e, QString &err );
QVariant    QXMLRPC_DECL parseXmlRpcStruct( const QDomElement &e, QString &err );
QVariant    QXMLRPC_DECL parseXmlRpcArray ( const QDomElement &e, QString &err );

QVariantMap QXMLRPC_DECL getFaultCode     ( const QVariant& fc);

/*
 * Types from http://www.xmlrpc.com/spec maps into QVariant as: int - int, boolean
 * - bool, string - QString, double double, dateTime.iso8601 - QDateTime, base64 -
 * QByteArray array - QVariantList struct - QVariantMap Fault method response
 * generated, if return value is QVariantMap with two fields: faultCode,
 * faultString. ;
 * creates xmlrpc fault ;
 * Use this in your callbacks, as return createFault( code, msg );
 * For example, in python, xmlrpclib.Fault exception will be raised.
 */
QVariant QXMLRPC_DECL   createFault ( const int code, const QString &msg );

#endif
