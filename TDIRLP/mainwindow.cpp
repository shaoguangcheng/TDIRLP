
#include <stdio.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    QChar s = addC->getSign();

    char *fmt = new char [100];
    if(!equal(c.xCoef, 0.0)){
        sprintf(fmt, "%3.2fx", c.xCoef);
    }

    if(greaterThan(c.yCoef, 0.0)){
        if(s == '+'){
            if(!equal(c.xCoef, 0.0))
                sprintf(fmt, "%s + %3.2fy", fmt, c.yCoef);
            else
                sprintf(fmt, "%3.2fy", c.yCoef);
        }
        else{
            if(!equal(c.xCoef, 0.0))
                sprintf(fmt, "%s - %3.2fy", fmt, c.yCoef);
            else{
                sprintf(fmt, "-");
                sprintf(fmt, "%s%3.2fy", fmt, c.yCoef);
            }
            c.yCoef *= -1.0;
        }
    }
    else
        if(lessThan(c.yCoef, 0.0)){
            if(s == '+'){
                if(!equal(c.xCoef, 0.0))
                    sprintf(fmt, "%s - %3.2fy", fmt, c.yCoef*-1.0);
                else
                    sprintf(fmt, "%3.2fy", c.yCoef);
            }
            else{
                 c.yCoef *= -1.0;
                 if(!equal(c.xCoef, 0.0))
                    sprintf(fmt, "%s + %3.2fy", fmt, c.yCoef);
                 else
                    sprintf(fmt, "%3.2fy", c.yCoef);
            }
        }

    sprintf(fmt, "%s <= %3.2f\n", fmt, c.bias);

    constraintStr.push_back(QString(fmt));
    ui->constrainsLabel->clear();
    ui->constrainsLabel->setText(constraintStr);

    constraint cTmp(c.xCoef, c.yCoef, -1.0*c.bias);
    tdlp->addConstraint(cTmp);

    plotFigure();

    delete [] fmt;
    delete addC;
}

void MainWindow::on_solveButton_clicked()
{

}

void MainWindow::on_clearButton_clicked()
{
    tdlp->clear();
    ui->constrainsLabel->clear();
    ui->objFunSignComboBox->setCurrentIndex(0);
    ui->xCoefLineEdit->setText(QString("0"));
    ui->yCoefLineEdit->setText(QString("0"));
    ui->solutionLabel->clear();
    ui->customplot->clearGraphs();
}

void MainWindow::on_exportResultButton_clicked()
{

}

void MainWindow::on_helpLinkButton_clicked()
{
    QString helpText = "";
    QMessageBox::about(this, "help", helpText);
}

void MainWindow::plotFigure()
{}
