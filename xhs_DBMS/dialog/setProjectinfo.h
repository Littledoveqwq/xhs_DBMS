#ifndef SETPROJECTINFO_H
#define SETPROJECTINFO_H

#include "InputBaseDialog.h"

namespace Ui {
class setProjectinfo;
}

class setProjectinfo : public InputBaseDialog
{
    Q_OBJECT

public:
    explicit setProjectinfo(QWidget *parent = nullptr);
    ~setProjectinfo();

private slots:
    void on_pushButton_clicked();

private:
    Ui::setProjectinfo *ui;
};

#endif // SETPROJECTINFO_H
