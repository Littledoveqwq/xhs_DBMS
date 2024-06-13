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
    ui->cbx_searchMode->setView(new QListView());

    std::function<void(QComboBox*, const QString&)> initCbx = [](QComboBox* cbx, const QString& text) {
        cbx->setView(new QListView());
        cbx->lineEdit()->setPlaceholderText(text);
        cbx->lineEdit()->setReadOnly(true);
        cbx->setCurrentIndex(-1);
    };

    QStringList types = SQLMgr::getInstance()->getAllBloggerTypes();
    for(auto& type : types)
        ui->cbx_type->addItem(type);

    initCbx(ui->cbx_fans, "粉丝量");
    initCbx(ui->cbx_type, "类型");
    initCbx(ui->cbx_noteprice, "图文价格");
    initCbx(ui->cbx_videoprice, "视频价格");
}

void AppBaseWindow::initTableView()
{
    QString queryStr;
    QStringList headers;
    //初始化博主资源表
    queryStr = SQLMgr::getInstance()->getQueryStr("bloggers_resource");
    headers = SQLMgr::getInstance()->getTableHeader("bloggers_resource");
    _bloggers_model = initSingleTable(queryStr, headers, ui->table_infoQuery);
    //设置自定义委托
    ui->table_infoQuery->setItemDelegate(new LinkDelegate(_bloggers_model, ui->table_infoQuery));
    ui->table_infoQuery->setMouseTracking(true);

    //初始化项目表
    queryStr = SQLMgr::getInstance()->getQueryStr("all_projectList").arg(_current_user);
    headers = SQLMgr::getInstance()->getTableHeader("all_projectList");
    _projects_model = initSingleTable(queryStr, headers, ui->table_recent);
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

        DBOperation::DBOperationResult res = SQLMgr::getInstance()->insertBloggersinfo(bloggerInfo);
        if(res == DBOperation::DBOperationResult::SUCCESS)
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

        subTab->getProjectId(prjName);
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


void AppBaseWindow::on_edt_Projectsearch_textChanged(const QString &arg1)
{
    QString query_projectInfo = QString("SELECT project_name, project_manager "
                                        " FROM project_info "
                                        "WHERE project_manager = '%1' AND project_name LIKE '%%2%' "
                                        "ORDER BY project_update_time DESC")
                                    .arg(_current_user) // managerName 是你的 project_manager 值
                                    .arg(arg1);

    _projects_serch_model = new MySqlQueryModel();
    _projects_serch_model->setQuery(query_projectInfo);

    if (_projects_serch_model->lastError().isValid()){
        qDebug() << "Failed to execute query:" << _projects_model->lastError().text();
    }

    setTableHeader(_projects_serch_model, {"project_name", "project_manager"});

    ui->table_projectSearch->setModel(_projects_serch_model);

    // 设置复选框列的宽度为50
    ui->table_projectSearch->setColumnWidth(CHECK_BOX_COLUMN, 10);
}


void AppBaseWindow::on_table_projectSearch_doubleClicked(const QModelIndex &index)
{
    if(index.column() == 1){
        QString prjName = _projects_serch_model->data(index, Qt::DisplayRole).toString();

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

        subTab->getProjectId(prjName);
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


void AppBaseWindow::on_btn_search_clicked()
{
    QStringList header = SQLMgr::getInstance()->getTableHeader("bloggers_resource");
    QString search_text = ui->edt_exactSearch->text();
    QString queryStr;
    if(ui->rbtn_exactSearch->isChecked()){
        if(ui->cbx_searchMode->currentText() == "博主昵称") {
            queryStr =
                SQLMgr::getInstance()->getQueryStr("bloggers_resource_by_nickname").arg(search_text);
        }
        else if (ui->cbx_searchMode->currentText() == "ID") {
            queryStr =
                SQLMgr::getInstance()->getQueryStr("bloggers_resource_by_ID").arg(search_text);
        }
        else if (ui->cbx_searchMode->currentText() == "微信") {
            queryStr =
                SQLMgr::getInstance()->getQueryStr("bloggers_resource_by_wechat").arg(search_text);
        }
    }
    else if (ui->rbtn_fuzzySearch->isChecked()) {
        queryStr =
            SQLMgr::getInstance()->getQueryStr("bloggers_resource");

        if(ui->cbx_fans->currentIndex() == -1
            && ui->cbx_noteprice->currentIndex() == -1
            && ui->cbx_type->currentIndex() == -1
            && ui->cbx_videoprice->currentIndex() == -1) {
            on_btn_clearSearch_clicked();
            return;
        }

        // 开始构建条件
        QStringList conditions;

        // 图文价格范围
        QString notePriceCondition;
        if (ui->cbx_noteprice->currentIndex() != -1) {
            QString notePrice = ui->cbx_noteprice->currentText();

            if(!notePrice.contains("-")) {
                notePriceCondition = QString("blogger_noteprice > %1").arg(50000);
            } else {
                int minNotePrice = parseNumRange(notePrice).first;
                int maxNotePrice = parseNumRange(notePrice).second;
                notePriceCondition = QString("blogger_noteprice BETWEEN %1 AND %2")
                                         .arg(minNotePrice)
                                         .arg(maxNotePrice);
            }
        }
        if (!notePriceCondition.isEmpty()) {
            conditions.append(notePriceCondition);
        }

        // 视频价格范围
        QString videoPriceCondition;
        if (ui->cbx_videoprice->currentIndex() != -1) {
            QString videoPrice = ui->cbx_videoprice->currentText();

            if(!videoPrice.contains("-")) {
                videoPriceCondition = QString("blogger_videoprice > %1").arg(50000);
            } else {
                int minVideoPrice = parseNumRange(videoPrice).first;
                int maxVideoPrice = parseNumRange(videoPrice).second;
                videoPriceCondition = QString("blogger_videoprice BETWEEN %1 AND %2")
                                         .arg(minVideoPrice)
                                         .arg(maxVideoPrice);
            }
        }
        if (!videoPriceCondition.isEmpty()) {
            conditions.append(videoPriceCondition);
        }

        // 粉丝量范围
        QString fansCondition;
        if (ui->cbx_fans->currentIndex() != -1) {
            QString fans = ui->cbx_fans->currentText();

            if(!fans.contains("-")) {
                fansCondition = QString("blogger_fans > %1").arg(1000000);
            } else {
                int minFans = parseNumRange(fans).first;
                int maxFans = parseNumRange(fans).second;
                fansCondition = QString("blogger_fans BETWEEN %1 AND %2")
                                          .arg(minFans)
                                          .arg(maxFans);
            }
        }
        if (!fansCondition.isEmpty()) {
            conditions.append(fansCondition);
        }

        // 类型筛选（如果有选择的话）
        if (ui->cbx_type->currentIndex() != -1) {
            QString selectedType = ui->cbx_type->currentText();
            QString typeCondition = QString("blogger_type LIKE '%%1%'").arg(selectedType);
            conditions.append(typeCondition);
        }

        // 拼接所有条件
        if (!conditions.isEmpty()) {
            queryStr += " WHERE " + conditions.join(" AND ");
        }
    }
    _bloggers_model = initSingleTable(queryStr, header, ui->table_infoQuery);
    //设置自定义委托
    ui->table_infoQuery->setItemDelegate(new LinkDelegate(_bloggers_model, ui->table_infoQuery));
    ui->table_infoQuery->setMouseTracking(true);

}

std::pair<int, int> AppBaseWindow::parseNumRange(const QString& priceRange) {
    QStringList parts = priceRange.split('-');
    if (parts.size() != 2) {
        qWarning() << "Invalid price range format:" << priceRange;
        return {0, 0}; // 返回默认值或者其他错误处理
    }

    bool conversionSuccess = false;
    int minPrice = parts[0].toInt(&conversionSuccess);
    if (!conversionSuccess) {
        qWarning() << "Failed to convert min price:" << parts[0];
        return {0, 0}; // 返回默认值或者其他错误处理
    }

    int maxPrice = parts[1].toInt(&conversionSuccess);
    if (!conversionSuccess) {
        qWarning() << "Failed to convert max price:" << parts[1];
        return {0, 0}; // 返回默认值或者其他错误处理
    }

    return {minPrice, maxPrice};
}

void AppBaseWindow::on_btn_clearSearch_clicked()
{
    ui->edt_exactSearch->clear();

    initComboBox();

    QString queryStr;
    QStringList headers;
    //初始化博主资源表
    queryStr = SQLMgr::getInstance()->getQueryStr("bloggers_resource");
    headers = SQLMgr::getInstance()->getTableHeader("bloggers_resource");
    _bloggers_model = initSingleTable(queryStr, headers, ui->table_infoQuery);
    //设置自定义委托
    ui->table_infoQuery->setItemDelegate(new LinkDelegate(_bloggers_model, ui->table_infoQuery));
    ui->table_infoQuery->setMouseTracking(true);
}


void AppBaseWindow::on_btn_importProject_clicked()
{
    // 创建主窗口
    QDialog *window = new QDialog(); // 创建动态分配的QDialog对象
    QFont font("黑体", 10);
    window->setFont(font);
    window->setWindowTitle("项目选择");

    // 创建垂直布局管理器
    QVBoxLayout *layout = new QVBoxLayout(window);

    // 创建项目名Label并添加到布局中
    QLabel *labelProjectName = new QLabel("项目名:", window);
    layout->addWidget(labelProjectName);

    // 创建ComboBox并添加项目
    QComboBox *comboBoxProjects = new QComboBox(window);
    comboBoxProjects->setStyleSheet("QComboBox QAbstractItemView {"
                                    "   min-height: 20px;"
                                    "   padding: 2px;"
                                    "   background-color: white;" // 设置背景颜色
                                    "   border: 1px solid gray;"   // 设置边框
                                    "}");
    comboBoxProjects->setView(new QListView());
    QStringList prjNames = SQLMgr::getInstance()->getAllProjectName();
    for(auto& prjName : prjNames)
        comboBoxProjects->addItem(prjName);
    layout->addWidget(comboBoxProjects);

    // 创建确认按钮并添加到布局中
    QPushButton *btnConfirm = new QPushButton("确认", window);
    layout->addWidget(btnConfirm);

    // 设置控件的固定大小
    labelProjectName->setFixedSize(50, 20);  // 设置Label大小为100x30像素
    comboBoxProjects->setMinimumSize(50, 30); // 设置ComboBox大小为200x30像素
    btnConfirm->setMinimumSize(50, 30);         // 设置按钮大小为80x30像素

    // 连接按钮点击事件的槽函数
    QObject::connect(btnConfirm, &QPushButton::clicked, this, [=]() {
        QString selectedProject = comboBoxProjects->currentText();

        qDebug() << "Selected project: " << selectedProject;

        // 在某个地方获取选中的行索引列表
        QList<int> selectedRows = _bloggers_model->getCheckedRows();
        QStringList existList;
        int id_col = 2;
        bool isInsert = false;
        for(auto& row : selectedRows){
            QString blogger_id = _bloggers_model->index(row, id_col).data().toString();
            DBOperation::DBOperationResult res =
                SQLMgr::getInstance()->insertBloggersToProject(selectedProject, blogger_id);

            if(res == DBOperation::DATA_EXIST) {
                existList.append(blogger_id);
            } else {
                isInsert = true;
            }
        }

        // 在你的方法或函数中使用以下代码
        QString warningMessage;
        if (!existList.isEmpty() && !isInsert) {
            warningMessage = "以下博主已经存在于项目中:\n";
            for (const QString& bloggerId : existList) {
                warningMessage += bloggerId + "\n";
            }
            warningMessage += "已经存在\n没有任何博主添加到项目中";
        } else if (!existList.isEmpty() && isInsert) {
            warningMessage = "以下博主已经存在于项目中:\n";
            for (const QString& bloggerId : existList) {
                warningMessage += bloggerId + "\n";
            }
            warningMessage += "已经存在\n其余博主成功添加到项目中";
        } else{
            warningMessage = "所有博主成功添加到项目中";
        }

        QMessageBox::warning(this, "警告", warningMessage);

        // 确认按钮点击后关闭窗口
        window->accept();
    });

    // 设置主窗口的布局
    window->setLayout(layout);

    // 设置主窗口的固定大小
    window->setFixedSize(200, 150); // 设置窗口大小为300x150像素

    window->exec(); // 使用exec()方法展示对话框
}

