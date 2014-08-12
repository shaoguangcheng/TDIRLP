#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextEdit>

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
    void plotFigure(const constraint &c);
    void setRange(QVector<double> vx, QVector<double> vy);
    void setText(const vertex& v, const QString& text );
    void initPlane();

    double getXValue(const constraint&c, double y);
    double getYValue(const constraint&c, double x);

    bool hasUpperBooundary(double val, double margin = 0.0) const;
    bool hasLowerBooundary(double val, double margin = 0.0) const;

private:
    Ui::MainWindow *ui;

    TDLP *tdlp;
    polygon *feasibleRegion;
    addConstraints *addC;

    QString constraintStr;
};

#endif // MAINWINDOW_H
