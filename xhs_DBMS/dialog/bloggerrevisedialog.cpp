#include "bloggerrevisedialog.h"
#include "ui_bloggerrevisedialog.h"

BloggerReviseDialog::BloggerReviseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BloggerReviseDialog)
{
    ui->setupUi(this);
}

BloggerReviseDialog::~BloggerReviseDialog()
{
    delete ui;
}

void BloggerReviseDialog::setInfo(BloggerInfo bloggerInfo)
{
    ui->edt_name->setText(bloggerInfo.blogger_nickname);
    ui->edt_type->setText(bloggerInfo.blogger_type);
    ui->edt_fans->setText(QString::number(bloggerInfo.blogger_fans));
    ui->edt_noteprice->setText(QString::number(bloggerInfo.blogger_noteprice));
    ui->edt_contact->setText(bloggerInfo.blogger_wechat);
    ui->edt_id->setText(bloggerInfo.blogger_id);
    ui->edt_likes->setText(QString::number(bloggerInfo.blogger_likes));
    ui->edt_videoprice->setText(QString::number(bloggerInfo.blogger_videoprice));
    ui->edt_homelink->setText(bloggerInfo.blogger_homelink);
}
