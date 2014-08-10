#include "addconstraints.h"
#include "ui_addconstraints.h"

addConstraints::addConstraints(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addConstraints)
{
    ui->setupUi(this);

    c.clear();
}

addConstraints::~addConstraints()
{
    delete ui;
}

void addConstraints::on_ensureButton_clicked()
{
    bool ok;

    c.clear();
    sign = '\0';

    c.xCoef = ui->xCoedLineEdit->text().toDouble(&ok);
    if(ok == false){
        QMessageBox::critical(this, "Error", tr("input error"));
        c.clear();
        return;
    }

    c.yCoef = ui->yCoefLineEdit->text().toDouble(&ok);
    if(ok == false){
        QMessageBox::critical(this, "Error", tr("input error"));
        c.clear();
        return;
    }

    c.bias  = ui->biasLineEdit->text().toDouble(&ok);
    if(ok == false){
        QMessageBox::critical(this, "Error", tr("input error"));
        c.clear();
        return;
    }

    int ch = ui->firstComboBox->currentIndex();
    if(ch == 0)
        sign = '+';
    if(ch == 1)
        sign = '-';

    if(equal(c.xCoef, 0)&&equal(c.yCoef, 0)){
        QMessageBox::critical(this, "Error", tr("xCoef and yCoef can not be zero meanwhile"));
        c.clear();
    }

    this->close();
}

void addConstraints::on_cancelButton_clicked()
{
    this->close();
}

constraint addConstraints::getConstraint() const
{
    return c;
}

QChar addConstraints::getSign() const
{
    return sign;
}
