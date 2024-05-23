#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_listWidget_currentTextChanged(const QString &currentText)
{
    if(currentText == "项目管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_projectManage);
    }
    else if(currentText=="消息群发")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_massageSend);
    }
    else if(currentText=="资源库管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_dbManage);
    }
    else if(currentText=="平台网站")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_webList);
    }
    else if(currentText=="数据爬取")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_dataFetcher);
    }
    else if(currentText=="信息采集")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_infoCollector);
    }
}


void Widget::on_rbtn_exactSearch_clicked()
{

}

