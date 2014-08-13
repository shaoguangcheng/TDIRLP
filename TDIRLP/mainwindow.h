#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextEdit>
#include <QDialog>

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
    /**
     * @brief on_addConstraintButton_clicked add constraint to LP
     */
    void on_addConstraintButton_clicked();

    /**
     * @brief on_solveButton_clicked solve the LP
     */
    void on_solveButton_clicked();

    /**
     * @brief on_clearButton_clicked clear all the items
     */
    void on_clearButton_clicked();

    /**
     * @brief on_exportResultButton_clicked export result to a pdf file
     */
    void on_exportResultButton_clicked();

    /**
     * @brief on_helpLinkButton_clicked do some help
     */
    void on_helpLinkButton_clicked();

    void mouseMoveEvent(QMouseEvent *);

private :
    /**
     * @brief setupCustomPlot do some setting for customplot
     * @param plot
     */
    void setupCustomPlot(QCustomPlot *plot);

    /**
     * @brief plotFigure plot constraint c on the customplot
     * @param c
     */
    void plotFigure(const constraint &c);

    /**
     * @brief setRange set the range of x axis and y axis for customplot
     * @param vx
     * @param vy
     */
    void setRange(QVector<double> vx, QVector<double> vy);

    /**
     * @brief setText put text on position v
     * @param v
     * @param text
     */
    void setText(const vertex& v, const QString& text );

    /**
     * @brief initPlane init the original plane
     */
    void initPlane();

    /**
     * @brief getXValue given y, calculate x according to function c
     * @param c
     * @param y
     * @return
     */
    double getXValue(const constraint&c, double y);

    /**
     * @brief getYValue given x, calculate y according to function c
     * @param c
     * @param x
     * @return
     */
    double getYValue(const constraint&c, double x);

    /**
     * @brief hasUpperBooundary does val has upper boundary
     * @param val
     * @param margin
     * @return
     */
    bool hasUpperBooundary(double val, double margin = 0.0) const;

    /**
     * @brief hasLowerBooundary does val has lower boundary
     * @param val
     * @param margin
     * @return
     */
    bool hasLowerBooundary(double val, double margin = 0.0) const;

private:
    Ui::MainWindow *ui;

    TDLP *tdlp;
    polygon *feasibleRegion;
    addConstraints *addC;

    QString constraintStr;
};

#endif // MAINWINDOW_H
