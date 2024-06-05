#include "prjinfodialog.h"
#include "ui_prjinfodialog.h"

PrjInfoDialog::PrjInfoDialog(QWidget *parent)
    : InputBaseDialog(parent)
    , ui(new Ui::PrjInfoDialog)
{
    ui->setupUi(this);
    setTopWidget(ui->topWidget);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

PrjInfoDialog::~PrjInfoDialog()
{
    delete ui;
}

void PrjInfoDialog::on_pushButton_clicked()
{
    reject();
}

