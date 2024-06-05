#ifndef PRJINFODIALOG_H
#define PRJINFODIALOG_H

#include "InputBaseDialog.h"

namespace Ui {
class PrjInfoDialog;
}

class PrjInfoDialog : public InputBaseDialog
{
    Q_OBJECT

public:
    explicit PrjInfoDialog(QWidget *parent = nullptr);
    ~PrjInfoDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PrjInfoDialog *ui;
};

#endif // PRJINFODIALOG_H
