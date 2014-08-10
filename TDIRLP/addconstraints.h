#ifndef ADDCONSTRAINTS_H
#define ADDCONSTRAINTS_H

#include <QDialog>
#include <QMessageBox>

#include "TDLP.h"

namespace Ui {
class addConstraints;
}

class addConstraints : public QDialog
{
    Q_OBJECT
    
public:
    explicit addConstraints(QWidget *parent = 0);
    ~addConstraints();

    constraint getConstraint() const;
    QChar getSign() const;
    
private slots:
    void on_ensureButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::addConstraints *ui;
    constraint c;
    QChar sign;
};

#endif // ADDCONSTRAINTS_H
