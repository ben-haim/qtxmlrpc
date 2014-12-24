/*******************************************************************************
*  file    :
*  created : 19.12.2014
*  author  :
*******************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

class NullStream
{
public:
    template <typename T>
    NullStream& operator<<(const T&) {return *this;}
};

class NotNullStreamDebug : public NullStream
{
public:
    template <typename T>
    NotNullStreamDebug& operator<<(const T& t) {qDebug() << t;return *this;}
};

class NotNullStreamWarning
{
public:
    template <typename T>
    NotNullStreamWarning& operator<<(const T& t) {qWarning() << t;return *this;}
};

class NotNullStreamError
{
public:
    template <typename T>
    NotNullStreamError& operator<<(const T& t) {qCritical() << t;return *this;}
};

inline NullStream make_stream(int i)
{
    switch (i)
        {
        case 0: return NullStream();
        case 1: return NotNullStreamDebug();
        }

    return NullStream();
}


#define LOG_IF_DEBUG(X)  (X) ? NotNullStreamDebug() : NullStream()
#define LOG_IF_WARNING(X)  (X) ? NotNullStreamWarning() : NullStream()
#define LOG_IF_ERROR(X)  (X) ? NotNullStreamError() : NullStream()

#define LOG_IF_TEST(X) (X) ? make_stream(1) : make_stream(0)

#endif // LOGGER_H

