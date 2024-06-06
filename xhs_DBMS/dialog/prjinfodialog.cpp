#include "prjinfodialog.h"
#include "ui_prjinfodialog.h"
#include "appbasewindow.h"
#include "sqlmgr.h"
#include <QDateTime>


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

bool PrjInfoDialog::projectinfo_to_sql(const QString &craftproject_name, const QString &manager, const QString &remarks)
{
    QSqlQuery query;

    QDateTime update_time = QDateTime::currentDateTime();
    QString format_update_time = update_time.toString("yyyy-MM-dd hh:mm:ss");
    query.prepare("INSERT INTO project_info(project_name, project_manager, project_remark,project_update_time) VALUES (:project_name, :project_manager, :project_remark, :project_update_time)");
    query.bindValue(":project_name", craftproject_name);
    query.bindValue(":project_manager", manager);
    query.bindValue(":project_remark", remarks);
    query.bindValue(":project_update_time",format_update_time);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "err", "添加失败，请重新输入");
        return false;
    }
    else
    {
        QMessageBox::information(nullptr, "success", "添加成功");
        return true;
    }
}

void PrjInfoDialog::on_buttonBox_accepted()
{
    QString project_name = ui->edt_projectName->text();
    QString month = ui->edt_carryMonth->text();
    QString manager=ui->edt_projectManager->text();
    QString remarks = ui->edt_remarkInfo->text();
    QString craftproject_name = "【" + month + "】" + project_name;

    if(projectinfo_to_sql(craftproject_name, manager, remarks)==true);
    {

    }
}




