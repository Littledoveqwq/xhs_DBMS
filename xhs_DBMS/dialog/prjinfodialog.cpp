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

void PrjInfoDialog::on_buttonBox_accepted()
{
    //优化设想: 自定义月份spinBox
    QString project_name = ui->edt_projectName->text();
    QString month = ui->edt_carryMonth->text();
    QString manager=ui->edt_projectManager->text();
    QString remark = ui->edt_remarkInfo->text().trimmed().isEmpty() ? "无" : ui->edt_remarkInfo->text();
    QString craftproject_name = "【" + month + "】" + project_name;   //合成后的项目名

    QDateTime update_time = QDateTime::currentDateTime();
    QString str_update_time = update_time.toString("yyyy-MM-dd hh:mm:ss");
    ProjectInfo prjInfo{craftproject_name, manager, remark, str_update_time};

    InsertData::InsertResult res = SQLMgr::getInstance()->createProject(prjInfo);
    if(res == InsertData::InsertResult::SUCCESS)
    {
        emit sig_create_prj();
    }
    else if(res == InsertData::InsertResult::MANAGER_NOT_EXIST) {
        QMessageBox::critical(this, "错误", "项目所有者不存在");
    }
}




