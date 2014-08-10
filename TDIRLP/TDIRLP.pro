TEMPLATE = app


QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

SOURCES += main.cpp \
    base.cpp \
    util.cpp \
    test.cpp \
    TDLP.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    addconstraints.cpp

HEADERS += \
    base.h \
    util.h \
    test.h \
    TDLP.h \
    qcustomplot.h \
    mainwindow.h \
    addconstraints.h

FORMS += \
    mainwindow.ui \
    addconstraints.ui

