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


    void on_buttonBox_accepted();

    bool projectinfo_to_sql(const QString &craftproject_name, const QString &manager, const QString &remarks);

private:
    Ui::PrjInfoDialog *ui;
};

#endif // PRJINFODIALOG_H
