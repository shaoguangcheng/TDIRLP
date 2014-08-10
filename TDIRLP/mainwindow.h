#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "TDLP.h"
#include "qcustomplot.h"
#include "addconstraints.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_addConstraintButton_clicked();
    void on_solveButton_clicked();
    void on_clearButton_clicked();
    void on_exportResultButton_clicked();
    void on_helpLinkButton_clicked();

private :
    void setupCustomPlot(QCustomPlot *plot);
    void plotFigure();

private:
    Ui::MainWindow *ui;

    TDLP *tdlp;
    polygon *feasibleRegion;
    addConstraints *addC;

    QString constraintStr;
};

#endif // MAINWINDOW_H
