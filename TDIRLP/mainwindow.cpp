#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupCustomPlot(ui->customplot);

    tdlp = new TDLP();
    feasibleRegion = new polygon();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupCustomPlot(QCustomPlot *plot)
{
    plot->setBackground(QBrush(QColor(150,150,150)));
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables |
                                    QCP::iSelectLegend | QCP::iSelectAxes);

    /**
     * set axis
     */
//    plot->xAxis->setRange();
//    plot->yAxis->setRange();
    plot->axisRect()->setupFullAxesBox();
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");

    QFont legendFont = font();
    plot->legend->setVisible(true);
    plot->legend->setFont(legendFont);
    plot->legend->setSelectedFont(legendFont);
    plot->legend->setSelectedParts(QCPLegend::spItems);
}

void MainWindow::on_addConstraintButton_clicked()
{
    addC = new addConstraints(this);
    addC->exec();

    constraint c = addC->getConstraint();

    delete addC;
}

void MainWindow::on_solveButton_clicked()
{

}

void MainWindow::on_clearButton_clicked()
{

}

void MainWindow::on_exportResultButton_clicked()
{

}

void MainWindow::on_helpLinkButton_clicked()
{

}

