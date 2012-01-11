/*******************************************************************************
*  file    : mainwindow.cpp
*  created : 11.01.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
