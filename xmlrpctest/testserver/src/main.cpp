#include <QCoreApplication>
#include "test_server.h"

int main( int argc, char **argv )
{
    QCoreApplication    app( argc, argv );
    QString             address= "127.0.0.1";
    quint16             port= 8080;
    TServer             s( address, port );
    return app.exec();
}
