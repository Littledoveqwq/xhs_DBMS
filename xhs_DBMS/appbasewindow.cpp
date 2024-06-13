#include "appbasewindow.h"
#include "ui_appbasewindow.h"
#include "dialog/prjinfodialog.h"
#include "dialog/bloggerrevisedialog.h"
#include <QDebug>
#include "sqlmgr.h"
#include <QDialogButtonBox>
#include "sqlmgr.h"
#include "projectmanagewidget.h"

AppBaseWindow::AppBaseWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppBaseWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page_projectManage);
    ui->tabWidget_resourceManage->setCurrentWidget(ui->tab_infoQuery);
    ui->stkWidget_search->setCurrentWidget(ui->page_exactSearch);
    ui->rbtn_exactSearch->setChecked(true);

    // 初始化 ComboBox 使其样式生效
    initComboBox();

    //初始化tabwidget
    initTabWidget();

    initReviseTable();

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}
AppBaseWindow::~AppBaseWindow()
{
    delete ui;
}

void AppBaseWindow::setUserName(const QString &username)
{
    _current_user = username;
    ui->username_label->setText(_current_user);
    ui->username_label->adjustSize();

    if(!_current_user.trimmed().isEmpty()) {
        //初始化 tableView
        initTableView();
    }
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
    //博主模型
    _bloggers_model = new MySqlQueryModel("bloggers_info");
    //项目模型
    _projects_model = new MySqlQueryModel("project_info");

    // 执行查询并将结果加载到 TableView 中
    _bloggers_model->setQuery("SELECT "
                              "blogger_nickname, blogger_id, blogger_type, "
                              "blogger_homelink, blogger_fans, blogger_likes, "
                              "blogger_noteprice, blogger_videoprice, blogger_wechat FROM bloggers_info");
    if (_bloggers_model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << _bloggers_model->lastError().text();
    }


    QString query_projectInfo = "SELECT project_name,project_manager,project_remark,"
                    "project_update_time FROM project_info WHERE project_manager = '%1' "
                    "ORDER BY project_update_time DESC";
    query_projectInfo = query_projectInfo.arg(_current_user);
    qDebug() << query_projectInfo;
    _projects_model->setQuery(query_projectInfo);
    if (_projects_model->lastError().isValid()){
        qDebug() << "Failed to execute query:" << _projects_model->lastError().text();
    }

    // 更新表头显示为中文
    setHeader(_bloggers_model);
    setHeader(_projects_model, {"", "项目名称", "项目管理人", "备注", "更新时间"});

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
}

void AppBaseWindow::initTabWidget()
{
    _Tab = new MyTabWidget(); // 将 Tab 定义为类的成员变量
    _Tab->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    _Tab->resize(this->size());
    _Tab->setTabsClosable(true); // 设置标签页上的标签为可关闭
    _Tab->hide(); // 隐藏 QTabWidget
    connect(_Tab, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QString PrjName = _Tab->tabText(index);
        _prjManageTabMap.remove(PrjName);
        _Tab->removeTab(index); // 移除指定索引的标签页

        if(_prjManageTabMap.isEmpty()) {
            _Tab->hide();
        }
    });

    connect(_Tab, &MyTabWidget::sig_tabWidget_close,this,&AppBaseWindow::slot_handle_tabWidget_close);
}

void AppBaseWindow::initReviseTable()
{
    _revise_model = new QStandardItemModel;

    // 从数据库中读取 JSON 数据
    QSqlQuery query;
    query.exec("SELECT revise_json FROM bloggers_info WHERE to_review = 'true'");
    QString jsonData;
    QStringList jsonList;
    while (query.next()) {
        jsonData = query.value(0).toString();
        jsonList.append(jsonData);
    }

    QStringList header = tr("姓名,编号,类型,主页链接,粉丝数,点赞数,"
                                         "笔记价格,视频价格,微信").simplified().split(",");

    _revise_model->setHorizontalHeaderLabels(header);
    for(auto& json : jsonList) {
        BloggerInfo blginfo;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
        if(jsonDoc.isObject()) {
            blginfo.fromJson(jsonDoc.object());
        }

        QList<QStandardItem *> row;
        row.append(new QStandardItem(blginfo.blogger_nickname));
        row.append(new QStandardItem(blginfo.blogger_id));
        row.append(new QStandardItem(blginfo.blogger_type));
        row.append(new QStandardItem(blginfo.blogger_homelink));
        row.append(new QStandardItem(QString::number(blginfo.blogger_fans)));
        row.append(new QStandardItem(QString::number(blginfo.blogger_likes)));
        row.append(new QStandardItem(QString::number(blginfo.blogger_noteprice)));
        row.append(new QStandardItem(QString::number(blginfo.blogger_videoprice)));
        row.append(new QStandardItem(blginfo.blogger_wechat));

        _revise_model->appendRow(row);
    }

    ui->table_infoReview->setModel(_revise_model);
    ui->table_infoReview->setItemDelegate(new LinkDelegate(_revise_model, this));
    ui->table_infoReview->setMouseTracking(true);
    ui->table_infoReview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

        BloggerInfo bloggerInfo{nickname,id,type,homelink,int_fans,int_likes,
                                int_noteprice,int_videoprice,contact};

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
    // 获取行数据
    QStringList info;
    int row = index.row();
    QStringList rowData;
    for (int col = 1; col < _bloggers_model->columnCount(); ++col) {
        QModelIndex cellIndex = _bloggers_model->index(row, col);
        info.append(cellIndex.data().toString());
    }

    // 构造 BloggerInfo 对象
    BloggerInfo bloggerInfo{info[0],info[1],info[2],info[3],info[4].toInt(),
                            info[5].toInt(),info[6].toInt(),info[7].toInt(),info[8]};

    // 创建 BloggerReviseDialog 对话框并设置数据
    BloggerReviseDialog* dialog = new BloggerReviseDialog();
    dialog->setInfo(bloggerInfo);

    // 如果对话框被接受，重新初始化修订表
    if(dialog->exec() == QDialog::Accepted) {
        initReviseTable();
        _bloggers_model->refresh();
    }
}

void AppBaseWindow::on_table_recent_doubleClicked(const QModelIndex &index)
{

    if(index.column() == 1){
        QString prjName = _projects_model->data(index, Qt::DisplayRole).toString();

        if(_prjManageTabMap.find(prjName) != _prjManageTabMap.end()) {
            qDebug() << "project has opened!";
            if (_Tab->windowState() & Qt::WindowMinimized) {
                _Tab->setWindowState(_Tab->windowState() & ~Qt::WindowMinimized);
            }
            _Tab->activateWindow();
            return;
        }
        ProjectManageWidget* subTab = new ProjectManageWidget();

        QSqlQuery query_prjId;
        query_prjId.prepare("SELECT project_id FROM project_info WHERE project_name = :project_name");
        query_prjId.bindValue(":project_name", prjName);

        if (!query_prjId.exec()) {
            qDebug() << "Failed to execute query:" << query_prjId.lastError().text();
            return;
        }

        subTab->setLabelText(prjName);


        auto index = _Tab->addTab(subTab, prjName);
        _prjManageTabMap.insert(prjName, static_cast<ProjectManageWidget*>(_Tab->widget(index)));
        _Tab->setCurrentIndex(index);
        if(!_Tab->isVisible()) _Tab->show();
        //设置焦点
        if (_Tab->windowState() & Qt::WindowMinimized) {
            _Tab->setWindowState(_Tab->windowState() & ~Qt::WindowMinimized);
        }
        _Tab->activateWindow();

        moveToCenter(_Tab);
    }
}

void AppBaseWindow::slot_handle_tabWidget_close()
{
    _prjManageTabMap.clear();
    _Tab->removeAllTabs();
}

void AppBaseWindow::closeEvent(QCloseEvent *event)
{
    if (_Tab) {
        _Tab->close();
    }
    QWidget::closeEvent(event);
}



void AppBaseWindow::on_btn_reviewPass_clicked()
{

    for(int row = _revise_model->rowCount() - 1; row >= 0; row--) {
        QStringList info;
        for (int col = 0; col < _revise_model->columnCount(); col++) {
            QModelIndex cellIndex = _revise_model->index(row, col);
            info.append(cellIndex.data().toString());
        }
        BloggerInfo blginfo{info[0],info[1],info[2],info[3],info[4].toInt(),
                            info[5].toInt(),info[6].toInt(),info[7].toInt(),info[8],""};

        QSqlQuery query;
        QString queryStr = "UPDATE bloggers_info SET blogger_nickname = :nickname, "
                           "blogger_type = :type, blogger_homelink = :homelink, "
                           "blogger_fans = :fans, blogger_likes = :likes, "
                           "blogger_noteprice = :noteprice, blogger_videoprice = :videoprice, "
                           "blogger_wechat = :wechat, revise_json = NULL, "
                           "to_review = 'false' WHERE blogger_id = :bloggerId";

        query.prepare(queryStr);
        query.bindValue(":nickname", blginfo.blogger_nickname);
        query.bindValue(":type", blginfo.blogger_type);
        query.bindValue(":homelink", blginfo.blogger_homelink);
        query.bindValue(":fans", blginfo.blogger_fans);
        query.bindValue(":likes", blginfo.blogger_likes);
        query.bindValue(":noteprice", blginfo.blogger_noteprice);
        query.bindValue(":videoprice", blginfo.blogger_videoprice);
        query.bindValue(":wechat", blginfo.blogger_wechat);
        query.bindValue(":bloggerId", blginfo.blogger_id);

        if(!query.exec()) {
            qDebug() << "Failed to update, userid is" + blginfo.blogger_id + ":" << query.lastError().text();
            continue;
        } else {
            qDebug() << "update success";
        }
        _revise_model->removeRow(row);
    }
    _bloggers_model->refresh();
}

