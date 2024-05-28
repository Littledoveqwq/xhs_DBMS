#include "setProjectinfo.h"
#include "ui_setProjectinfo.h"

setProjectinfo::setProjectinfo(QWidget *parent)
    : InputBaseDialog(parent)
    , ui(new Ui::setProjectinfo)
{
    ui->setupUi(this);
    setTopWidget(ui->topWidget);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

setProjectinfo::~setProjectinfo()
{
    delete ui;
}

void setProjectinfo::on_pushButton_clicked()
{
    reject();
}

