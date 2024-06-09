#include "projectmanagewidget.h"
#include "ui_projectmanagewidget.h"


ProjectManageWidget::ProjectManageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectManageWidget)
{
    ui->setupUi(this);
}

void ProjectManageWidget::setLabelText(const QString &text)
{
    ui->lbl_projectName->setText(text);
}


ProjectManageWidget::~ProjectManageWidget()
{
    delete ui;
}
