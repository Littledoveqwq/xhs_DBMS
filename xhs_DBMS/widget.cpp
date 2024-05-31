#include "widget.h"
#include "ui_widget.h"
#include "dialog/setProjectinfo.h"
#include <QDebug>
#include "sqlmgr.h"

// TODO 把所有映射表建立起来
// TODO 将CustomSqlQueryModel 这个委托类封装好
// TODO 完成上面两步以后来整理widget的构造函数
///@test
/// 创建一个映射表，将英文字段名映射到中文表头
QMap<QString, QString> columnMapping = {
    {"id", "编号"},
    {"name", "姓名"},
    {"type", "类型"},
    {"link", "链接"},
    {"fans", "粉丝数"},
    {"likes", "点赞数"},
    {"noteprice", "笔记价格"},
    {"vedioprice", "视频价格"},
    {"wechat", "微信"},
    {"Remark", "备注"}
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

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    auto sqlmgr = SQLMgr::getInstance();

    // 初始化 ComboBox 使其样式生效
    initComboBox();

    // 创建自定义的模型对象
    CustomSqlQueryModel *model = new CustomSqlQueryModel(this);

    // 执行查询并将结果加载到 TableView 中
    model->setQuery("SELECT * FROM bloggers_info");
    if (model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << model->lastError().text();
        delete model;
        return;
    }

    // 更新表头显示为中文
    for (int col = 0; col < model->columnCount(); ++col) {
        QString englishHeader = model->headerData(col, Qt::Horizontal).toString();
        QString chineseHeader = columnMapping.value(englishHeader, englishHeader);
        model->setHeaderData(col, Qt::Horizontal, chineseHeader);
    }

    // 设置自定义模型到表格视图中
    ui->table_infoQuery->setModel(model);

    // 取消自动换行
    ui->table_infoQuery->setWordWrap(false);
    // 超出文本不显示省略号
    ui->table_infoQuery->setTextElideMode(Qt::ElideNone);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initComboBox()
{
    ui->cbx_exactSearch->setView(new QListView());
    ui->cbx_fans->setView(new QListView());
    ui->cbx_kind->setView(new QListView());
    ui->cbx_noteprice->setView(new QListView());
    ui->cbx_vedioprice->setView(new QListView());
}

void Widget::on_listWidget_currentTextChanged(const QString &currentText)
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


void Widget::on_rbtn_exactSearch_clicked()
{
    ui->stkWidget_search->setCurrentWidget(ui->page_exactSearch);
}


void Widget::on_rbtn_fuzzySearch_clicked()
{
    ui->stkWidget_search->setCurrentWidget(ui->page_fuzzySearch);
}


void Widget::on_btn_projectNew_clicked()
{
    setProjectinfo setProjectinfoDialog(this);
    setProjectinfoDialog.exec();
}

void Widget::open_dialog_setProjectinfo()
{
    projectinfo = new setProjectinfo;
    projectinfo->show();
}


void Widget::on_rbt_recent_clicked()
{
    ui->stackedWidget_projectList->setCurrentWidget(ui->page_recent);
}


void Widget::on_rbt_favour_clicked()
{
    ui->stackedWidget_projectList->setCurrentWidget(ui->page_favour);
}

