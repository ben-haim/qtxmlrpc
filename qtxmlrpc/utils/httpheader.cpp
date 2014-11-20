/*******************************************************************************
*  file    : httpheader.cpp
*  created : 23.07.2013
*  author  : 
*******************************************************************************/

#include "httpheader.h"
#include <QSet>

HttpHeader::HttpHeader() : valid_(true)
{
}

HttpHeader::HttpHeader(const HttpHeader& header)
{
    valid_  = header.valid_ ;
    values_ = header.values_;
}

HttpHeader::HttpHeader(const QString& str)
{
    valid_ = true;
    parse(str);
}

HttpHeader::~HttpHeader()
{
}

HttpHeader&HttpHeader::operator=(const HttpHeader& h)
{
    values_ = h.values_;
    valid_  = h.valid_;
    return *this;
}

void HttpHeader::setValue(const QString& key, const QString& value)
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::Iterator it = values_.begin();
    while (it != values_.end()) {
        if ((*it).first.toLower() == lowercaseKey) {
            (*it).second = value;
            return;
        }
        ++it;
    }
    // not found so add
    addValue(key, value);
}

void HttpHeader::setValues(const QList<QPair<QString, QString> >& val)
{
    values_ = val;
}

void HttpHeader::addValue(const QString& key, const QString& value)
{
    values_.append(qMakePair(key, value));
}

QList<QPair<QString, QString> > HttpHeader::values() const
{
    return values_;
}

bool HttpHeader::hasKey(const QString& key) const
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::ConstIterator it = values_.constBegin();
    while (it != values_.constEnd()) {
        if ((*it).first.toLower() == lowercaseKey)
            return true;
        ++it;
    }
    return false;
}

QStringList HttpHeader::keys() const
{
    QStringList keyList;
    QSet<QString> seenKeys;
    QList<QPair<QString, QString> >::ConstIterator it = values_.constBegin();
    while (it != values_.constEnd()) {
        const QString &key = (*it).first;
        QString lowercaseKey = key.toLower();
        if (!seenKeys.contains(lowercaseKey)) {
            keyList.append(key);
            seenKeys.insert(lowercaseKey);
        }
        ++it;
    }
    return keyList;
}

QString HttpHeader::value(const QString& key) const
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::ConstIterator it = values_.constBegin();
    while (it != values_.constEnd()) {
        if ((*it).first.toLower() == lowercaseKey)
            return (*it).second;
        ++it;
    }
    return QString();
}

QStringList HttpHeader::allValues(const QString& key) const
{
    QString lowercaseKey = key.toLower();
    QStringList valueList;
    QList<QPair<QString, QString> >::ConstIterator it = values_.constBegin();
    while (it != values_.constEnd()) {
        if ((*it).first.toLower() == lowercaseKey)
            valueList.append((*it).second);
        ++it;
    }
    return valueList;
}

void HttpHeader::removeValue(const QString& key)
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::Iterator it = values_.begin();
    while (it != values_.end()) {
        if ((*it).first.toLower() == lowercaseKey) {
            values_.erase(it);
            return;
        }
        ++it;
    }
}

void HttpHeader::removeAllValues(const QString& key)
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::Iterator it = values_.begin();
    while (it != values_.end()) {
        if ((*it).first.toLower() == lowercaseKey) {
            it = values_.erase(it);
            continue;
        }
        ++it;
    }
}

bool HttpHeader::hasContentLength() const
{
    return hasKey(QLatin1String("content-length"));
}

uint HttpHeader::contentLength() const
{
    return value(QLatin1String("content-length")).toUInt();
}

void HttpHeader::setContentLength(int len)
{
    setValue(QLatin1String("content-length"), QString::number(len));
}

bool HttpHeader::hasContentType() const
{
    return hasKey(QLatin1String("content-type"));
}

QString HttpHeader::contentType() const
{
    QString type = value(QLatin1String("content-type"));
    if (type.isEmpty())
        return QString();

    int pos = type.indexOf(QLatin1Char(';'));
    if (pos == -1)
        return type;

    return type.left(pos).trimmed();
}

void HttpHeader::setContentType(const QString& type)
{
    setValue(QLatin1String("content-type"), type);
}

QString HttpHeader::toString() const
{
    if (!isValid())
        return QLatin1String("");

    QString ret = QLatin1String("");

    QList<QPair<QString, QString> >::ConstIterator it = values_.constBegin();
    while (it != values_.constEnd()) {
        ret += (*it).first + QLatin1String(": ") + (*it).second + QLatin1String("\r\n");
        ++it;
    }
    return ret;
}

bool HttpHeader::isValid() const
{
    return valid_;
}

bool HttpHeader::parseLine(const QString& line, int )
{
    int i = line.indexOf(QLatin1Char(':'));
    if (i == -1)
        return false;

    addValue(line.left(i).trimmed(), line.mid(i + 1).trimmed());

    return true;
}

bool HttpHeader::parse(const QString& str)
{
    QStringList lst;
    int pos = str.indexOf(QLatin1Char('\n'));
    if (pos > 0 && str.at(pos - 1) == QLatin1Char('\r'))
        lst = str.trimmed().split(QLatin1String("\r\n"));
    else
        lst = str.trimmed().split(QLatin1String("\n"));
    lst.removeAll(QString()); // No empties

    if (lst.isEmpty())
        return true;

    QStringList lines;
    QStringList::Iterator it = lst.begin();
    for (; it != lst.end(); ++it) {
        if (!(*it).isEmpty()) {
            if ((*it)[0].isSpace()) {
                if (!lines.isEmpty()) {
                    lines.last() += QLatin1Char(' ');
                    lines.last() += (*it).trimmed();
                }
            } else {
                lines.append((*it));
            }
        }
    }

    int number = 0;
    it = lines.begin();
    for (; it != lines.end(); ++it) {
        if (!parseLine(*it, number++)) {
            valid_ = false;
            return false;
        }
    }
    return true;
}

void HttpHeader::setValid(bool v)
{
    valid_ = v;
}


HttpResponseHeader::HttpResponseHeader()
    : HttpHeader()
{
    setValid(false);
}

HttpResponseHeader::HttpResponseHeader(const HttpResponseHeader& header) :
    HttpHeader(header)
{
    statCode  = header.statCode;
    reasonPhr = header.reasonPhr;
    majVer    = header.majVer;
    minVer    = header.minVer;
}

HttpResponseHeader::HttpResponseHeader(const QString& str)
    : HttpHeader()
{
    parse(str);
}

HttpResponseHeader::HttpResponseHeader(int code, const QString& text, int majorVer, int minorVer)
    : HttpHeader()
{
    setStatusLine(code, text, majorVer, minorVer);
}

HttpResponseHeader&HttpResponseHeader::operator=(const HttpResponseHeader& header)
{
    HttpHeader::operator=(header);
    statCode  = header.statCode;
    reasonPhr = header.reasonPhr;
    majVer    = header.majVer;
    minVer    = header.minVer;
    return *this;
}

void HttpResponseHeader::setStatusLine(int code, const QString& text, int majorVer, int minorVer)
{
    setValid(true);
    statCode  = code;
    reasonPhr = text;
    majVer    = majorVer;
    minVer    = minorVer;
}

int HttpResponseHeader::statusCode() const
{
    return statCode;
}

QString HttpResponseHeader::reasonPhrase() const
{
    return reasonPhr;
}

int HttpResponseHeader::majorVersion() const
{
    return majVer;
}

int HttpResponseHeader::minorVersion() const
{
    return minVer;
}

QString HttpResponseHeader::toString() const
{
    QString ret(QLatin1String("HTTP/%1.%2 %3 %4\r\n%5\r\n"));
    return ret.arg(majVer)
              .arg(minVer)
              .arg(statCode)
              .arg(reasonPhr)
              .arg(HttpHeader::toString());
}

bool HttpResponseHeader::parseLine(const QString& line, int number)
{
    if (number != 0)
        return HttpHeader::parseLine(line, number);

    QString l = line.simplified();
    if (l.length() < 10)
        return false;

    if (l.left(5) == QLatin1String("HTTP/") && l[5].isDigit() && l[6] == QLatin1Char('.') &&
        l[7].isDigit() && l[8] == QLatin1Char(' ') && l[9].isDigit()) {
        majVer = l[5].toLatin1() - '0';
        minVer = l[7].toLatin1() - '0';

        int pos = l.indexOf(QLatin1Char(' '), 9);
        if (pos != -1) {
            reasonPhr = l.mid(pos + 1);
            statCode = l.mid(9, pos - 9).toInt();
        } else {
            statCode = l.mid(9).toInt();
            reasonPhr.clear();
        }
    } else {
        return false;
    }

    return true;
}





HttpRequestHeader::HttpRequestHeader() :
    HttpHeader()
{
    setValid(false);
}

HttpRequestHeader::HttpRequestHeader(const QString& method, const QString& path,
                                     int majorVer, int minorVer) :
    HttpHeader()
{
    m      = method;
    p      = path;
    majVer = majorVer;
    minVer = minorVer;
}

HttpRequestHeader::HttpRequestHeader(const HttpRequestHeader& header) :
    HttpHeader(header)
{
    m      = header.m;
    p      = header.p;
    majVer = header.majVer;
    minVer = header.minVer;
}

HttpRequestHeader::HttpRequestHeader(const QString& str) :
    HttpHeader()
{
    parse(str);
}

HttpRequestHeader&HttpRequestHeader::operator=(const HttpRequestHeader& header)
{
    HttpHeader::operator=(header);
    m      = header.m;
    p      = header.p;
    majVer = header.majVer;
    minVer = header.minVer;
    return *this;
}

void HttpRequestHeader::setRequest(const QString& method, const QString& path, int majorVer, int minorVer)
{
    setValid(true);
    m      = method;
    p      = path;
    majVer = majorVer;
    minVer = minorVer;
}

QString HttpRequestHeader::method() const
{
    return m;
}

QString HttpRequestHeader::path() const
{
    return p;
}

int HttpRequestHeader::majorVersion() const
{
    return majVer;
}

int HttpRequestHeader::minorVersion() const
{
    return minVer;
}

QString HttpRequestHeader::toString() const
{
    QString first(QLatin1String("%1 %2"));
    QString last(QLatin1String(" HTTP/%3.%4\r\n%5\r\n"));
    return first.arg(m).arg(p) +
        last.arg(majVer).arg(minVer).arg(HttpHeader::toString());
}

bool HttpRequestHeader::parseLine(const QString& line, int number)
{
    if (number != 0)
        return HttpHeader::parseLine(line, number);

    QStringList lst = line.simplified().split(QLatin1String(" "));
    if (lst.count() > 0) {
        m = lst[0];
        if (lst.count() > 1) {
            p = lst[1];
            if (lst.count() > 2) {
                QString v = lst[2];
                if (v.length() >= 8 && v.left(5) == QLatin1String("HTTP/") &&
                    v[5].isDigit() && v[6] == QLatin1Char('.') && v[7].isDigit()) {
                    majVer = v[5].toLatin1() - '0';
                    minVer = v[7].toLatin1() - '0';
                    return true;
                }
            }
        }
    }

    return false;
}
