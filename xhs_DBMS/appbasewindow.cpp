#include "appbasewindow.h"
#include "ui_appbasewindow.h"
#include "dialog/prjinfodialog.h"
#include <QDebug>
#include "sqlmgr.h"
#include <QDialogButtonBox>

// TODO 将CustomSqlQueryModel 这个委托类封装好
// TODO 完成上面两步以后来整理widget的构造函数

AppBaseWindow::AppBaseWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppBaseWindow)
{
    ui->setupUi(this);

    // 初始化 ComboBox 使其样式生效
    initComboBox();
    //初始化 tableView
    initTableView();

    //博主模型
    _bloggers_model = new CustomSqlQueryModel;
    //项目模型
    _projects_model = new CustomSqlQueryModel;

    // 执行查询并将结果加载到 TableView 中
    _bloggers_model->setQuery("SELECT * FROM bloggers_info");
    if (_bloggers_model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << _bloggers_model->lastError().text();
    }

    _projects_model->setQuery("SELECT project_name,project_manager,project_remark,"
                              "project_update_time FROM project_info ORDER BY project_update_time DESC");
    if (_projects_model->lastError().isValid()){
        qDebug() << "Failed to execute query:" << _projects_model->lastError().text();
    }

    // 更新表头显示为中文
    for (int col = 0; col < _bloggers_model->columnCount(); ++col) {
        QString englishHeader = _bloggers_model->headerData(col, Qt::Horizontal).toString();
        QString chineseHeader = columnMapping.value(englishHeader, englishHeader);
        _bloggers_model->setHeaderData(col, Qt::Horizontal, chineseHeader);
    }

    for (int col = 0; col < _projects_model->columnCount(); ++col) {
        QString englishHeader = _projects_model->headerData(col, Qt::Horizontal).toString();
        QString chineseHeader = columnMapping.value(englishHeader, englishHeader);
        _projects_model->setHeaderData(col, Qt::Horizontal, chineseHeader);
    }

    // 设置自定义模型到表格视图中
    ui->table_infoQuery->setModel(_bloggers_model);
    ui->table_recent->setModel(_projects_model);

    // 取消自动换行
    ui->table_infoQuery->setWordWrap(false);
    ui->table_recent->setWordWrap(false);
    // 超出文本不显示省略号
    ui->table_infoQuery->setTextElideMode(Qt::ElideNone);
    ui->table_recent->setTextElideMode(Qt::ElideNone);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}

AppBaseWindow::~AppBaseWindow()
{
    delete ui;
}

void AppBaseWindow::setUserName(const QString &username)
{
    ui->username_label->setText(username);
    ui->username_label->adjustSize();
}

void AppBaseWindow::initComboBox()
{
    ui->cbx_exactSearch->setView(new QListView());
    ui->cbx_fans->setView(new QListView());
    ui->cbx_kind->setView(new QListView());
    ui->cbx_noteprice->setView(new QListView());
    ui->cbx_vedioprice->setView(new QListView());
}

void AppBaseWindow::initTableView()
{

}

void AppBaseWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    if(currentText == "项目管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_projectManage);
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

}


void AppBaseWindow::on_rbtn_exactSearch_clicked()
{
    ui->stkWidget_search->setCurrentWidget(ui->page_exactSearch);
}


void AppBaseWindow::on_rbtn_fuzzySearch_clicked()
{
    ui->stkWidget_search->setCurrentWidget(ui->page_fuzzySearch);
}


void AppBaseWindow::on_btn_projectNew_clicked()
{
    PrjInfoDialog* creatprjDialog = new PrjInfoDialog(this);
    connect(creatprjDialog, &PrjInfoDialog::sig_create_prj,
            this, &AppBaseWindow::slot_handle_create_project);
    creatprjDialog->exec();
    creatprjDialog->deleteLater();
}


void AppBaseWindow::on_rbt_recent_clicked()
{
    ui->stackedWidget_projectList->setCurrentWidget(ui->page_recent);
}


void AppBaseWindow::on_rbt_favour_clicked()
{
    ui->stackedWidget_projectList->setCurrentWidget(ui->page_favour);
}

void AppBaseWindow::slot_handle_create_project()
{
    _projects_model->refresh();
}

