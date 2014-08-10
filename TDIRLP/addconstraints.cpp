#include "addconstraints.h"
#include "ui_addconstraints.h"

addConstraints::addConstraints(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addConstraints)
{
    ui->setupUi(this);

    c.clear();
    sign.clear();
}

addConstraints::~addConstraints()
{
    delete ui;
}

void addConstraints::on_ensureButton_clicked()
{
    bool ok;

    c.clear();
    sign.clear();

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

    QChar ch = ui->firstComboBox->currentIndex();
    if(ch == 0)
        sign.push_back('+');
    if(ch == 1)
        sign.push_back('-');

    ch = ui->secondComboBox->currentIndex();
    if(ch == 0)
        sign.push_back('+');
    if(ch == 1)
        sign.push_back('-');

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

QString addConstraints::getSign() const
{
    return sign;
}
