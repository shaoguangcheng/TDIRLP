#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString f("\n2x + 5y <= 9\n");
    ui->label->setText(f);
    f.append("3x + 4y <= 0");
    ui->label->setText(f);


    ui->customplot->setBackground(QBrush(QColor(150,150,150)));
    ui->customplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->customplot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
