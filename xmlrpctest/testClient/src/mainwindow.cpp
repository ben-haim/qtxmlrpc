/*
 * file : mainwindow.cpp created : 11.01.2012 author : Slyshyk Oleksiy
 * (alex312@meta.ua
 */
#include <QDateTime>
#include <QApplication>
#include <xmlrpcclient.h>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    connect( ui->test_1_btn, SIGNAL(clicked()), this, SLOT(test_1()) );
    connect( ui->test_2_btn, SIGNAL(clicked()), this, SLOT(test_2()) );
    connect( ui->test_3_btn, SIGNAL(clicked()), this, SLOT(test_3()) );
    connect( ui->test_4_btn, SIGNAL(clicked()), this, SLOT(test_4()) );

    client_ = new XmlRpcClient( "localhost", 8080 );
    connect( client_, SIGNAL( dataReady( QVariant)), this, SLOT( onEcho( QVariant)) );
    connect( client_, SIGNAL( error ( const QString & )), this, SLOT( onError ( const QString & )) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test_1()
{
    for(int i = 0; i < 100; ++i)
        {
            ready_ = false;

            QVariant param = QVariant("Qt XML-RPC");

            XmlRpcClient    *client= new XmlRpcClient( "localhost", 8080 );
            connect( client, SIGNAL( dataReady( QVariant)), this, SLOT( onDataReady( QVariant)) );
            connect( client, SIGNAL( error ( const QString & )), this, SLOT( onError ( const QString & )) );
            client->execute( "testFunc", QVariantList() << param );
            while ( !ready_ )
              {
                qApp->processEvents();
              }
            delete client;
        }
}

void MainWindow::test_2()
{

    qDebug()<<"MainWindow::test_2()";
    QVariant param = QVariant(QDateTime::currentDateTime());
    client_->execute( "echo", QVariantList() << param );
    param = QVariant(QDateTime::currentDateTime());
    client_->execute( "echo", QVariantList() << param );
    param = QVariant(QDateTime::currentDateTime());
    client_->execute( "echo", QVariantList() << param );
    param = QVariant(QDateTime::currentDateTime());
    client_->execute( "echo", QVariantList() << param );
    param = QVariant(QDateTime::currentDateTime());
    client_->execute( "echo", QVariantList() << param );
}

void MainWindow::test_3()
{
    for(int i = 0; i < 100; ++i)
        {
            QVariant param = QVariant(QString("Qt XML-RPC TEST 3 - %1").arg(i));

            XmlRpcClient    *client= new XmlRpcClient( "localhost", 8080 );
            connect( client, SIGNAL( dataReady( QVariant)), this, SLOT( onDataReady( QVariant)) );
            connect( client, SIGNAL( error ( const QString & )), this, SLOT( onError ( const QString & )) );
            client->execute( "testFunc", QVariantList() << param );
        }
}

void MainWindow::test_4()
{
    for(int i = 0; i < 10; ++i)
        {
            QVariant param = QVariant(QString("Qt XML-RPC TEST 4 - %1").arg(i));
            client_->execute( "echo", QVariantList() << param );
            qApp->processEvents();
        }
}

void MainWindow::onDataReady ( const QVariant &response )
{
    qDebug() << response;
    ready_ = true;
}

void MainWindow::onEcho( const QVariant &response )
{
    qDebug() <<"Echo : "<< response;
}

void MainWindow::onError( const QString &errTxt )
{
    qDebug() << errTxt;
    ready_ = true;
}

#include "moc_mainwindow.cpp"
