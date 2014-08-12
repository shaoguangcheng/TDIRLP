
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

    initPlane();
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
//    plot->legend->setSelectedParts(QCPLegend::spItems);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));
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

    ui->customplot->clearPlottables();
    ui->customplot->clearItems();

    plotFigure(cTmp);

    delete [] fmt;
    delete addC;
}

void MainWindow::on_solveButton_clicked()
{
    bool ok;
    objFunc fun;

    fun.xCoef = ui->xCoefLineEdit->text().toDouble(&ok);
    if(ok == false){
        QMessageBox::critical(this, "Error", tr("objective funtion error"));
        fun.clear();
        return;
    }

    fun.yCoef = ui->yCoefLineEdit->text().toDouble(&ok);
    if(ok == false){
        QMessageBox::critical(this, "Error", tr("objective funtion error"));
        fun.clear();
        return;
    }

    if(equal(fun.xCoef, 0)&&equal(fun.yCoef, 0)){
        QMessageBox::critical(this, "Error", tr("the coefficient x ang y can not be zero in the same time"));
        fun.clear();
        return;
    }

    int index = ui->objFunSignComboBox->currentIndex();
    if(index == 1)
        fun.yCoef *= -1.0;
    tdlp->setObjFunc(fun);

    solution s = tdlp->solve();

    QString fmt;
    if(s.getStatus() == singlePoint){
        fmt = "optimal solution is a point.\n";
        fmt = fmt + QString("coordinate : (")
                + QString::number(s.getPoint().x, 'g', '2') + QString(", ")
                + QString::number(s.getPoint().y, 'g', '2') + QString(").\n");
        fmt = fmt + QString("optimal value : ") + QString::number(s.getFuncValue(), 'g', '2');

        ui->customplot->clearGraphs();
        ui->customplot->addGraph();
        ui->customplot->graph(0)->setPen(QPen(Qt::red));
        ui->customplot->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->customplot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
        ui->customplot->graph(0)->addData(s.getPoint().x, s.getPoint().y);

        QCPItemText *vertexLabel = new QCPItemText(ui->customplot);
        ui->customplot->addItem(vertexLabel);
        vertexLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignLeft);
        vertexLabel->position->setType(QCPItemPosition::ptPlotCoords);

        vertexLabel->position->setCoords(s.getPoint().x, s.getPoint().y);
        vertexLabel->setColor(QColor(255,0,0));
        vertexLabel->setText(QString("optimal point"));
    }

    if(s.getStatus() == singleLine){
        vertex start = s.getEdge().start, end = s.getEdge().end;

        fmt = "optimal solution is a line.\n";
        fmt = fmt + QString("from (") +
                QString::number(start.x, 'g', '2') + QString(", ")
                + QString::number(start.y, 'g', '2') + QString(") \nto (")
                + QString::number(end.x, 'g', '2') + QString(", ")
                + QString::number(end.y, 'g', '2') + QString(").\n");
        fmt = fmt + QString("optimal value : ") + QString::number(s.getFuncValue(), 'g', '2');

#if 0
        const int N = 2;
        QVector<double> vx(N), vy(N);

        vx.push_back(start.x);
        vx.push_back(end.x);
        vy.push_back(start.y);
        vy.push_back(end.y);

        ui->customplot->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->customplot->graph(0)->setData(vx, vy);
#endif

        QCPItemText *vertexLabel = new QCPItemText(ui->customplot);
        ui->customplot->addItem(vertexLabel);
        vertexLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignLeft);
        vertexLabel->position->setType(QCPItemPosition::ptPlotCoords);
        vertexLabel->setColor(QColor(255,0,0));

        vertexLabel->position->setCoords((start.x+end.x)/2, (start.y+end.y)/2);
        vertexLabel->setText(QString("optimal line"));
    }

    if(s.getStatus() == unbounded){
        fmt = QString("the feasible region is \nunbounded\n");
    }

    if(s.getStatus() == noSolution){
        fmt = QString("the two dimensional \nlinear programme\n has feasible solution");
    }

    ui->solutionLabel->clear();
    ui->solutionLabel->setText(fmt);

    ui->customplot->replot();
}

void MainWindow::on_clearButton_clicked()
{
    tdlp->clear();
    constraintStr.clear();
    feasibleRegion->clear();
    ui->constrainsLabel->clear();
    ui->objFunSignComboBox->setCurrentIndex(0);
    ui->xCoefLineEdit->setText(QString("0"));
    ui->yCoefLineEdit->setText(QString("0"));
    ui->solutionLabel->clear();
    ui->customplot->clearGraphs();
    ui->customplot->clearPlottables();
    ui->customplot->clearItems();
    ui->customplot->rescaleAxes();
    ui->customplot->replot();
}

void MainWindow::on_exportResultButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save document...", qApp->applicationDirPath(), "*.pdf");
    if (!fileName.isEmpty())
    {
      QPrinter printer;
      printer.setFullPage(true);
      printer.setPaperSize(QPrinter::A4);
      printer.setOrientation(QPrinter::Portrait);
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setOutputFileName(fileName);

      QTextEdit *textEdit = new QTextEdit;
      textEdit->append("objective function : ");
      textEdit->append(tdlp->getObjFunc().toString().c_str());
      textEdit->append("\n ");
      textEdit->append("constraints : ");
      textEdit->append(ui->constrainsLabel->text());
      textEdit->append("\n ");

      int width = ui->customplot->width()/2;
      int height = ui->customplot->height()/2;
      ui->customplot->savePng("1.png", width, height);

      textEdit->append("<img src = 1.png>");
      textEdit->append("\nsolution : ");
      textEdit->append(ui->solutionLabel->text());
      textEdit->document()->print(&printer);
     }
}

/**
 * @brief MainWindow::on_helpLinkButton_clicked help slot
 */
void MainWindow::on_helpLinkButton_clicked()
{
    QString helpText = "1. click add constraints button to add new constraint for the linear programming.\n\n"
            "2. after adding all the constraints, please input the objective funtion in the left top panel.\n\n"
            "3. click solve button to get the answer of the linear programming.\n\n"
            "4. the result will be displayed in the result panel.\n\n"
            "5. click export result button to save the problem and its answer to a specified file with pdf format.\n\n"
            "6. click the clear button to clean all the items in the UI and then you can input a new problem";
    QMessageBox::about(this, "help", helpText);
}

/**
 * @brief MainWindow::plotFigure plot c on customplot
 * @param c
 */
void MainWindow::plotFigure(const constraint& c)
{
#if 0
    QCPRange xRange = ui->customplot->xAxis->range();
    QCPRange yRange = ui->customplot->yAxis->range();

    double xLower = xRange.lower, yLower = yRange.lower;
    double xUpper = xRange.upper, yUpper = yRange.upper;
    const int N = 200;
    double xStep = (xUpper - xLower)/N;
    double yStep = (yUpper - yLower)/N;

    QVector<double> x(N), y(N);
    if(!equal(c.yCoef, 0))
        for(int i=0; i<N; i++){
            x[i] = xLower + i*xStep;
            y[i] = -1.0*(c.bias + c.xCoef*x[i])/c.yCoef;
        }
    else{
        for(int i=0; i<N; i++){
            x[i] = -1.0*c.bias/c.xCoef;
            y[i] = yLower + i*yStep;
        }
    }
#endif

    if(0 == feasibleRegion->vertice.size())
        initPlane();

    feasibleRegion->intersectOfHalfPlane(c);

    list<vertex> vertice = feasibleRegion->vertice;
    QVector<double> vx, vy;
    list<vertex>::iterator it;
    for(it = vertice.begin(); it != vertice.end(); it++){
        vx.push_back(it->x);
        vy.push_back(it->y);

        if(lessThan(it->x, UPBOUND) &&
                greaterThan(it->x, LOWBOUND) &&
                lessThan(it->y, UPBOUND) &&
                greaterThan(it->y, LOWBOUND)){
                QString text = "(" + QString::number(it->x, 'g', 2) + ", ";
                text = text + QString::number(it->y, 'g', 2) + ")";

                QCPItemText *vertexLabel = new QCPItemText(ui->customplot);
                ui->customplot->addItem(vertexLabel);
                vertexLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignRight);
                vertexLabel->position->setType(QCPItemPosition::ptPlotCoords);

                vertexLabel->position->setCoords(it->x,it->y);
                vertexLabel->setText(text);
            }
    }

    QCPCurve* curve = new QCPCurve(ui->customplot->xAxis, ui->customplot->yAxis);
    curve->setPen(QPen(QColor(Qt::blue)));
    curve->setBrush(QBrush(QColor(0, 255, 0, 20)));
    curve->setData(vx, vy);
    ui->customplot->addPlottable(curve);

    setRange(vx, vy);

    ui->customplot->replot();
}
/**
 * @brief MainWindow::setText put text at position v
 * @param v
 * @param text
 */
void MainWindow::setText(const vertex& v, const QString& text )
{
    QCPItemText *vertexLabel = new QCPItemText(ui->customplot);
    ui->customplot->addItem(vertexLabel);
    vertexLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignLeft);
    vertexLabel->position->setType(QCPItemPosition::ptPlotCoords);

    vertexLabel->position->setCoords(v.x, v.y);
    vertexLabel->setText(text);
}

/**
 * @brief MainWindow::initPlane initialize the original plane
 */
void MainWindow::initPlane()
{
    vertex v1(LOWBOUND, UPBOUND);
    vertex v2(UPBOUND, UPBOUND);
    vertex v3(UPBOUND, LOWBOUND);
    vertex v4(LOWBOUND, LOWBOUND);

    edge e1(v1, v2);
    edge e2(v2, v3);
    edge e3(v3, v4);
    edge e4(v4, v1);

    list<edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);

    feasibleRegion = new polygon(edges);
}

/**
 * @brief MainWindow::getYValue given x, calculate y according to function c
 * @param c
 * @param x
 * @return
 */
double MainWindow::getYValue(const constraint&c, double x)
{
    if(equal(c.yCoef, 0)){
        return 0;
    }
    else{
        return -1.0*(c.bias+c.xCoef*x)/c.yCoef;
    }
}

/**
 * @brief MainWindow::getXValue given y, calculate x according to function c
 * @param c
 * @param y
 * @return
 */
double MainWindow::getXValue(const constraint&c, double y)
{
    if(equal(c.xCoef, 0)){
        return 0;
    }
    else{
        return -1.0*(c.bias+c.yCoef*y)/c.xCoef;
    }
}

/**
 * @brief MainWindow::hasUpperBooundary  val has upper boundary ?
 * @param val
 * @param margin
 * @return
 */
bool MainWindow::hasUpperBooundary(double val, double margin) const
{
    return lessThan(val+margin, 1000);
}

/**
 * @brief MainWindow::hasLowerBooundary  val has lower boundary ?
 * @param val
 * @param margin
 * @return
 */
bool MainWindow::hasLowerBooundary(double val, double margin) const
{
    return greaterThan(val-margin, -1000);
}

/**
 * @brief MainWindow::setRange decide the range the x axis and y axis
 * @param vx
 * @param vy
 */
void MainWindow::setRange(QVector<double> vx, QVector<double> vy)
{
    sort(vx.begin(), vx.end());
    sort(vy.begin(), vy.end());
    double xMax = vx.back(), xMin = vx.front();
    double yMax = vy.back(), yMin = vy.front();
    double xMargin = (xMax - xMin)/10.0;
    double yMargin = (yMax - yMin)/10.0;

    double xUpper = 100, xLower = -100, yUpper = 100, yLower = -100;

    if(hasUpperBooundary(xMax, xMargin)&&hasLowerBooundary(xMin, xMargin)){
        xUpper = xMax + xMargin;
        xLower = xMin - xMargin;
    }

    if(hasUpperBooundary(xMax, xMargin)&&(!hasLowerBooundary(xMin, xMargin))){
        xUpper = xMax + fabs(xMax)/8+1.0;
        xLower = xMax - fabs(xMax)/8-1.0;
    }

    if((!hasUpperBooundary(xMax, xMargin))&&hasLowerBooundary(xMin, xMargin)){
        xUpper = xMin + fabs(xMin)/8+1.0;
        xLower = xMin - fabs(xMin)/8-1.0;
    }

    if((!hasUpperBooundary(xMax, xMargin))&&(!hasLowerBooundary(xMin, xMargin))){
        xUpper = 100;
        xLower = -100;
    }

    if(hasUpperBooundary(yMax, yMargin)&&hasLowerBooundary(yMin, yMargin)){
        yUpper = yMax + yMargin;
        yLower = yMin - yMargin;
    }

    if(hasUpperBooundary(yMax, yMargin)&&(!hasLowerBooundary(yMin, yMargin))){
        yUpper = yMax + fabs(yMax)/8+1.0;
        yLower = yMax - fabs(yMax)/8-1.0;
    }

    if((!hasUpperBooundary(yMax, xMargin))&&hasLowerBooundary(yMin, yMargin)){
        yUpper = yMin + fabs(yMin)/8+1.0;
        yLower = yMin - fabs(yMin)/8-1.0;
    }

    if((!hasUpperBooundary(yMax, yMargin))&&(!hasLowerBooundary(yMin, yMargin))){
        yUpper = xUpper;
        yLower = xLower;
    }

    ui->customplot->xAxis->setRange(xLower, xUpper);
    ui->customplot->yAxis->setRange(yLower, yUpper);

//    ui->customplot->rescaleAxes();
}
