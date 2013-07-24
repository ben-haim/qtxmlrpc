/*
 * file : mainwindow.hpp created : 11.01.2012
 * author : Slyshyk Oleksiy (alex312@meta.ua)
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QMainWindow>

QT_FORWARD_DECLARE_CLASS(XmlRpcClient)

namespace   Ui
{
class   MainWindow;
}

class MainWindow :
    public QMainWindow
{
    Q_OBJECT
public:
    explicit    MainWindow( QWidget *parent= 0 );
    ~           MainWindow();

private slots:
    void test_1();
    void test_2();
    void test_3();
    void test_4();
    void onDataReady ( const QVariant &response );
    void onEcho( const QVariant &response );
    void onError( const QString &errTxt );
private:
    Ui::MainWindow * ui;
    bool ready_;
    XmlRpcClient * client_;
};

#endif /* MAINWINDOW_HPP */
