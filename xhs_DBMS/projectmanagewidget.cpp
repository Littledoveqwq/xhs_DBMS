#include "projectmanagewidget.h"
#include "ui_projectmanagewidget.h"


ProjectManageWidget::ProjectManageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectManageWidget)
{
    ui->setupUi(this);

    QString queryStr = "SELECT pni.blogger_id, pni.note_link, pni.note_likes,"
                       " pni.note_collection, pni.note_remarks, pni.repay, pni.note_title,"
                       " bi.blogger_nickname, bi.blogger_id, bi.blogger_type, bi.blogger_homelink,"
                       " bi.blogger_fans, bi.blogger_likes, bi.blogger_noteprice, bi.blogger_videoprice,"
                       " bi.blogger_wechat FROM project_note_info pni JOIN bloggers_info bi ON pni.blogger_id = bi.blogger_id;";

    _model = new MySqlQueryModel;
    _model->setQuery(queryStr);
    if (_model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << _model->lastError().text();
    }
    ui->table_projectManage->setModel(_model);
}

void ProjectManageWidget::setLabelText(const QString &text)
{
    ui->lbl_projectName->setText(text);
    ui->lbl_projectName->adjustSize();
}


ProjectManageWidget::~ProjectManageWidget()
{
    delete ui;
}
