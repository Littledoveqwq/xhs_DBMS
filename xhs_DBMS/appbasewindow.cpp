#include "appbasewindow.h"
#include "ui_appbasewindow.h"
#include "dialog/prjinfodialog.h"
#include "dialog/bloggerrevisedialog.h"
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
    _bloggers_model = new MySqlQueryModel;
    //项目模型
    _projects_model = new MySqlQueryModel;

    // 执行查询并将结果加载到 TableView 中
    _bloggers_model->setQuery("SELECT blogger_nickname, blogger_id, blogger_type, "
                              "blogger_homelink, blogger_fans, blogger_likes, "
                              "blogger_noteprice, blogger_videoprice, blogger_wechat FROM bloggers_info");
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

    //设置自定义委托
    ui->table_infoQuery->setItemDelegate(new LinkDelegate(_bloggers_model, this));
    ui->table_infoQuery->setMouseTracking(true);

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

void AppBaseWindow::slot_handle_insert_blogger()
{
    _bloggers_model->refresh();
}
void AppBaseWindow::on_btn_upload_clicked()
{


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Upload Confirmation", "Do you want to upload the information?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 获取用户输入
        QString nickname= ui->edt_name->text();
        QString id = ui->edt_id->text();
        QString type= ui->edt_type->text();
        QString homelink= ui->edt_link->text();
        QString fans = ui->edt_fans->text();
        QString likes = ui->edt_likes->text();
        QString noteprice = ui->edt_noteprice->text();
        QString videoprice = ui->edt_videoprice->text();
        QString contact =ui->edt_contact->text();

        int int_fans = fans.toInt();
        int int_likes = likes.toInt();
        int int_noteprice = noteprice.toInt();
        int int_videoprice = videoprice.toInt();


        // 判断edt_id的输入是否为空
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Upload Failed", "添加失败，必须输入博主ID。");
            return;
        }

        // 判断输入的edt_fans, edt_likes, edt_noteprice, edt_videoprice是否为int类型
        bool isFansInt, isLikesInt, isNotepriceInt, isVideopriceInt;
        fans.toInt(&isFansInt);
        likes.toInt(&isLikesInt);
        noteprice.toInt(&isNotepriceInt);
        videoprice.toInt(&isVideopriceInt);

        if (!isFansInt || !isLikesInt || !isNotepriceInt || !isVideopriceInt) {
            QMessageBox::warning(this, "Upload Failed", "添加失败，粉丝，赞藏，价格必须为整数。");
            return;
        }

        // 如果所有检查都通过，可以进行上传操作
        // 在这里添加你的上传操作代码
        QMessageBox::information(this, "Upload Successful", "信息上传成功。");

        BloggerInfo bloggerInfo{nickname,id,type,homelink,int_fans,int_likes,int_noteprice,int_videoprice,contact};

        InsertData::InsertResult res = SQLMgr::getInstance()->insertBloggersinfo(bloggerInfo);
        if(res == InsertData::InsertResult::SUCCESS)
        {
            emit sig_insert_blogger();
        }
        else {
            QMessageBox::critical(this, "错误", "添加失败");
        }
    }
}

void AppBaseWindow::on_table_infoQuery_doubleClicked(const QModelIndex &index)
{
    //获取行数据
    QStringList info;
    //双击的行
    int row = index.row();
    // 获取整行的内容
    QStringList rowData;
    for (int col = 0; col < _bloggers_model->columnCount(); ++col) {
        QModelIndex cellIndex = _bloggers_model->index(row, col);
        info.append(cellIndex.data().toString());
    }

    BloggerInfo bloggerInfo{info[0],info[1],info[2],info[3],info[4].toInt(),
                            info[5].toInt(),info[6].toInt(),info[7].toInt(),info[8]};
    //双击行出现弹窗进行修改信息
    BloggerReviseDialog* dialog = new BloggerReviseDialog();
    dialog->setInfo(bloggerInfo);
    dialog->exec();
}

