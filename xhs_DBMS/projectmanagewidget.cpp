#include "projectmanagewidget.h"
#include "ui_projectmanagewidget.h"
#include "sqlmgr.h"

ProjectManageWidget::ProjectManageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectManageWidget)
{
    ui->setupUi(this);

    m_menu = new QMenu();
}

void ProjectManageWidget::setLabelText(const QString &text)
{
    ui->lbl_projectName->setText(text);
    ui->lbl_projectName->adjustSize();
}


///两个查询待封装
void ProjectManageWidget::getProjectId(const QString &text)
{
    QSqlQuery query;
    QString queryString = "SELECT project_id FROM project_info WHERE project_name = :project_name";
    query.prepare(queryString);
    query.bindValue(":project_name", text);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return; // 处理查询失败情况
    }

    if (query.next()) {
        int projectId = query.value(0).toInt();
        qDebug() << "Project ID:" << projectId;
        // 将 projectId 设置给 project_id 属性
        _project_id = projectId;
    } else {
        qDebug() << "No project found with name:" << text;
        // 处理未找到项目的情况
        return;
    }

    QSqlQuery queryTeamer;
    QString queryTeamerString = "SELECT ui.user_nickname "
                                "FROM project_teamer pt "
                                "JOIN user_info ui ON pt.user_id = ui.user_id "
                                "WHERE pt.project_id = :project_id;";
    queryTeamer.prepare(queryTeamerString);
    queryTeamer.bindValue(":project_id", _project_id);

    if (!queryTeamer.exec()) {
        qDebug() << "Failed to execute query:" << queryTeamer.lastError().text();
        return; // 处理查询失败情况
    }

    // 清空菜单项，确保不重复添加
    m_menu->clear();


    // 添加查询到的菜单项
    while (queryTeamer.next()) {
        QString userNickname = queryTeamer.value(0).toString();
        // 获取菜单项列表
        auto menuActions = m_menu->actions();
        if (std::none_of(menuActions.begin(), menuActions.end(),
                         [&userNickname](QAction *action) { return action->text() == userNickname; })) {
            m_menu->addAction(userNickname);
        }
    }

    QString queryStr = "SELECT pni.note_link, pni.note_likes,"
                       " pni.note_collection, pni.note_remarks, pni.repay, pni.public_time,"
                       " bi.blogger_nickname, bi.blogger_id, bi.blogger_type, bi.blogger_homelink,"
                       " bi.blogger_fans, bi.blogger_likes, bi.blogger_noteprice, bi.blogger_videoprice,"
                       " bi.blogger_wechat FROM project_note_info pni JOIN bloggers_info bi ON pni.blogger_id = bi.blogger_id "
                       "WHERE pni.project_id = %1";

    queryStr = queryStr.arg(_project_id);

    _model = new MySqlQueryModel();

    _model->setQuery(queryStr);
    if (_model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << _model->lastError().text();
    }

    setTableHeader(_model, {"note_link", "note_likes", "note_collection",
                      "note_remarks", "repay", "note_title", "blogger_nickname",
                      "blogger_id", "blogger_type", "blogger_homelink", "blogger_fans",
                      "blogger_likes", "blogger_noteprice", "blogger_videoprice",
                      "blogger_wechat"});
    ui->table_projectManage->setModel(_model);
}


ProjectManageWidget::~ProjectManageWidget()
{
    delete m_menu;
    delete ui;
}


void ProjectManageWidget::on_btn_projectTeamer_released()
{
    m_menu->exec(ui->btn_projectTeamer->mapToGlobal(QPoint(0, ui->btn_projectTeamer->height())));
}


void ProjectManageWidget::on_btn_bfUpload_clicked()
{
    QString filePath
        = QFileDialog::getOpenFileName(this, tr("选择文档"),
                                       "",
                                       tr("Word Files (*.docx);;Excel Files (*.xlsx);;Text Files (*.txt);;All Files (*.*)"));

    if (filePath.isEmpty()) {
        QMessageBox::information(this, tr("文件选择"), tr("未选择任何文件"));
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("错误"), tr("无法打开文件"));
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName(); // 获取完整文件名，包括扩展名

    // 将文件名和二进制数据传递给保存函数
    DBOperation::DBOperationResult res = SQLMgr::getInstance()->saveAttachmentToDatabase(_project_id, fileName, fileData);
    if(res == DBOperation::SUCCESS) {
        QMessageBox::information(this, "附件上传", "附件上传成功");
    } else if (res == DBOperation::DATA_EXIST) {
        QMessageBox::warning(this, "保存失败", "附件已经存在");
    }
}


void ProjectManageWidget::on_btn_bfDownload_clicked()
{
    // 获取项目附件文件名列表
    QStringList filenames = SQLMgr::getInstance()->getProjectAttachmentList(_project_id);

    // 创建菜单对象
    QMenu menu(this);

    // 遍历附件文件名列表，为每个文件名创建菜单项，并关联槽函数
    foreach (const QString& filename, filenames) {
        QAction* action = menu.addAction(filename);
        connect(action, &QAction::triggered, this, [=]() {
            // 获取菜单项的文本作为下载文件的文件名
            QString downloadFilename = action->text();

            // 下载对应的附件数据
            QByteArray attachmentData = SQLMgr::getInstance()->getAttachmentData(_project_id, downloadFilename);

            // 保存附件数据到文件
            QString saveFilePath = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath() + "/" + downloadFilename);
            if (!saveFilePath.isEmpty()) {
                QFile file(saveFilePath);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(attachmentData);
                    file.close();
                    QMessageBox::information(this, "文件保存", "文件已成功保存到：" + saveFilePath);
                } else {
                    QMessageBox::warning(this, "保存失败", "无法保存文件：" + saveFilePath);
                }
            }
        });
    }

    // 获取按钮的位置，将菜单显示在按钮旁边
    QPoint pos = ui->btn_bfDownload->mapToGlobal(QPoint(0, ui->btn_bfDownload->height()));
    menu.exec(pos);
}

void ProjectManageWidget::downloadAttachment(const QString &filename)
{
    // 示例代码：获取文件内容并保存到本地
    QByteArray fileData = SQLMgr::getInstance()->getAttachmentData(_project_id, filename);
    QString savePath = QFileDialog::getSaveFileName(this, tr("保存文件"), "", tr("All Files (*.*)"));
    if (!savePath.isEmpty()) {
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(fileData);
            file.close();
            QMessageBox::information(this, tr("提示"), tr("文件下载成功！"));
        } else {
            QMessageBox::critical(this, tr("错误"), tr("无法保存文件：%1").arg(file.errorString()));
        }
    }
}

