/* file    : main.cpp
 * created : 11.01.2012
 * author  : Slyshyk Oleksiy (alexSlyshyk@gmail.com)
 */


#include <QApplication>
#include "mainwindow.hpp"
int main( int argc, char *argv[] )
{
    QApplication    a( argc, argv );
    MainWindow      w;
    w.show();
    return a.exec();
}
