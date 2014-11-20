#ifndef XMLRPCCONV_H
#define XMLRPCCONV_H

#include <QVariant>
#include <QByteArray>
#include <QDomElement>

QByteArray          toXmlRpcRequest   ( const QString m, const QVariantList &l );
QVariant            fromXmlRpcResponse( const QString d,       QString      &err );

/* returns default http header for our xmlrpc server */
QByteArray          xmlRpcResponseHeader( const qint64 contentLength );

/* create xmlrpc response from QVariant */
QByteArray          toXmlRpcResponse( const QVariant &v );

/*
 * QVariant to xml conversions ;
 * use QByteArray & reference, becouse it is faster, then return QByteArray
 */
void                toXmlRpcValue ( const int spaces, const QVariant     &child, QByteArray &b );
void                toXmlRpcStruct( const int spaces, const QVariantMap  &child, QByteArray &b );
void                toXmlRpcArray ( const int spaces, const QVariantList &child, QByteArray &b );

/* xml to QVariant conversions */
QVariant            parseXmlRpcValue ( const QDomElement &e, QString &err );
QVariant            parseXmlRpcStruct( const QDomElement &e, QString &err );
QVariant            parseXmlRpcArray ( const QDomElement &e, QString &err );

QVariantMap         getFaultCode     ( const QVariant& fc);

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
inline QVariant createFault ( const int code, const QString &msg )
{
    QVariantMap f;
    f["faultCode"  ] = code;
    f["faultString"] = msg ;
    return f;
}

#endif
