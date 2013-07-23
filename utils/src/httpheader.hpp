/*******************************************************************************
*  file    : httpheader.hpp
*  created : 23.07.2013
*  author  : 
*******************************************************************************/

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <QString>
#include <QStringList>
#include <QPair>

class HttpHeader
{
public:
    HttpHeader();
    HttpHeader(const HttpHeader &header);
    HttpHeader(const QString &str);
    virtual ~HttpHeader();

    HttpHeader &operator=(const HttpHeader &h);

    void setValue(const QString &key, const QString &value);
    void setValues(const QList<QPair<QString, QString> > &values);
    void addValue(const QString &key, const QString &value);
    QList<QPair<QString, QString> > values() const;
    bool hasKey(const QString &key) const;
    QStringList keys() const;
    QString value(const QString &key) const;
    QStringList allValues(const QString &key) const;
    void removeValue(const QString &key);
    void removeAllValues(const QString &key);

    // ### Qt 5: change to qint64
    bool hasContentLength() const;
    uint contentLength() const;
    void setContentLength(int len);

    bool hasContentType() const;
    QString contentType() const;
    void setContentType(const QString &type);

    virtual QString toString() const;
    bool isValid() const;

    virtual int majorVersion() const = 0;
    virtual int minorVersion() const = 0;

protected:
    virtual bool parseLine(const QString &line, int number);
    bool parse(const QString &str);
    void setValid(bool);
private:
    QList<QPair<QString, QString> > values_;
    bool                            valid_;
};

class HttpResponseHeader : public HttpHeader
{
public:
    HttpResponseHeader();
    HttpResponseHeader(const HttpResponseHeader &header);
    HttpResponseHeader(const QString &str);
    HttpResponseHeader(int code, const QString &text = QString(), int majorVer = 1, int minorVer = 1);
    HttpResponseHeader &operator=(const HttpResponseHeader &header);

    void setStatusLine(int code, const QString &text = QString(), int majorVer = 1, int minorVer = 1);

    int statusCode() const;
    QString reasonPhrase() const;

    int majorVersion() const;
    int minorVersion() const;

    QString toString() const;

protected:
    bool parseLine(const QString &line, int number);

private:
    int statCode;
    QString reasonPhr;
    int majVer;
    int minVer;
};

class HttpRequestHeader : public HttpHeader
{
public:
    HttpRequestHeader();
    HttpRequestHeader(const QString &method, const QString &path, int majorVer = 1, int minorVer = 1);
    HttpRequestHeader(const HttpRequestHeader &header);
    HttpRequestHeader(const QString &str);
    HttpRequestHeader &operator=(const HttpRequestHeader &header);

    void setRequest(const QString &method, const QString &path, int majorVer = 1, int minorVer = 1);

    QString method() const;
    QString path() const;

    int majorVersion() const;
    int minorVersion() const;

    QString toString() const;

protected:
    bool parseLine(const QString &line, int number);

private:
    QString m;
    QString p;
    int majVer;
    int minVer;
};

#endif // HTTPHEADER_HPP
