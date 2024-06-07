#include "datareviewdialog.h"
#include "ui_datareviewdialog.h"

datareviewdialog::datareviewdialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::datareviewdialog)
{
    ui->setupUi(this);
}

datareviewdialog::~datareviewdialog()
{
    delete ui;
}
