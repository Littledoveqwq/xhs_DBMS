#include "appbasewindow.h"
#include "ui_appbasewindow.h"
#include "dialog/prjinfodialog.h"
#include <QDebug>
#include "sqlmgr.h"

// TODO 把所有映射表建立起来
// TODO 将CustomSqlQueryModel 这个委托类封装好
// TODO 完成上面两步以后来整理widget的构造函数
///@test
/// 创建一个映射表，将英文字段名映射到中文表头
QMap<QString, QString> columnMapping = {
    {"blogger_id", "编号"},
    {"blogger_nickname", "姓名"},
    {"blogger_type", "类型"},
    {"blogger_link", "链接"},
    {"blogger_fans", "粉丝数"},
    {"blogger_likes", "点赞数"},
    {"blogger_noteprice", "笔记价格"},
    {"blogger_vedioprice", "视频价格"},
    {"blogger_wechat", "微信"},
    {"blogger_Remark", "备注"},
    {"project_name","项目名称"},
    {"project_manager","项目管理人"},
    {"project_remark", "项目备注"},
    {"project_update_time","最近更新时间"}
};


class CustomSqlQueryModel : public QSqlQueryModel
{
public:
    CustomSqlQueryModel(QObject *parent = nullptr)
        : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        } else {
            return QSqlQueryModel::data(index, role);
        }
    }
};
///@test end

AppBaseWindow::AppBaseWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppBaseWindow)
{
    ui->setupUi(this);

    auto sqlmgr = SQLMgr::getInstance();

    // 初始化 ComboBox 使其样式生效
    initComboBox();

    // 创建自定义的模型对象
    //博主模型
    CustomSqlQueryModel *bloggers_model = new CustomSqlQueryModel(this);
    //项目模型
    CustomSqlQueryModel *projects_model = new CustomSqlQueryModel(this);

    // 执行查询并将结果加载到 TableView 中
    bloggers_model->setQuery("SELECT * FROM bloggers_info");
    if (bloggers_model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << bloggers_model->lastError().text();
        delete bloggers_model;
        return;
    }

    projects_model->setQuery("SELECT project_name,project_manager,project_remark,project_update_time FROM project_info ORDER BY project_update_time DESC");
    if (projects_model->lastError().isValid()){
        qDebug() << "Failed to execute query:" << projects_model->lastError().text();
        delete projects_model;
        return;
    }

    // 更新表头显示为中文
    for (int col = 0; col < bloggers_model->columnCount(); ++col) {
        QString englishHeader = bloggers_model->headerData(col, Qt::Horizontal).toString();
        QString chineseHeader = columnMapping.value(englishHeader, englishHeader);
        bloggers_model->setHeaderData(col, Qt::Horizontal, chineseHeader);
    }

    for (int col = 0; col < projects_model->columnCount(); ++col) {
        QString englishHeader = projects_model->headerData(col, Qt::Horizontal).toString();
        QString chineseHeader = columnMapping.value(englishHeader, englishHeader);
        projects_model->setHeaderData(col, Qt::Horizontal, chineseHeader);
    }

    // 设置自定义模型到表格视图中
    ui->table_infoQuery->setModel(bloggers_model);
    ui->table_recent->setModel(projects_model);

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
    PrjInfoDialog setProjectinfoDialog(this);
    setProjectinfoDialog.exec();
}

void AppBaseWindow::open_dialog_setProjectinfo()
{
    projectinfo = new PrjInfoDialog;
    projectinfo->show();
}


void AppBaseWindow::on_rbt_recent_clicked()
{
    ui->stackedWidget_projectList->setCurrentWidget(ui->page_recent);
}


void AppBaseWindow::on_rbt_favour_clicked()
{
    ui->stackedWidget_projectList->setCurrentWidget(ui->page_favour);
}

