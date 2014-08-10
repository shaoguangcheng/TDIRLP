#include <iostream>

#include <QApplication>

#include "addconstraints.h"
#include "mainwindow.h"
#include "test.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

//    testHalfPlane();
//    testPolygon();
    testTDLP();
//    testFindMax();
    return app.exec();
}

