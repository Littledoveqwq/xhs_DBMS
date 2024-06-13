#include "sqlmgr.h"
#include "configmgr.h"

SQLMgr::~SQLMgr() {
    disconnectFromDB();
}

SQLMgr::SQLMgr() :
    _query(QSqlQuery())
{
    connectToDB();
    registerQueryMap();
    registerTableHeaderMap();
}

MySQLState::SQLConnState SQLMgr::connectToDB() {
    MySQLState::SQLConnState res = MySQLState::SUCCESS;

    MySQLInfo sqlInfo = ConfigMgr::getInstance()->getMysqlInfo();

    _db = QSqlDatabase::addDatabase("QMYSQL");
    _db.setHostName(sqlInfo.host);
    _db.setPort(sqlInfo.port);
    _db.setDatabaseName(sqlInfo.database_name);
    _db.setUserName(sqlInfo.user_name);
    _db.setPassword(sqlInfo.passwd);

    bool ok = _db.open();
    if(ok) {
        qDebug() << "SQL_CONN_SUCCESS";
        _query = QSqlQuery(_db);
    } else {
        qDebug() << "SQL_CONN_FAILURE: " << _db.lastError();
        res = MySQLState::FAILURE;
    }

    return res;
}

MySQLState::SQLConnState SQLMgr::disconnectFromDB() {
    MySQLState::SQLConnState res = MySQLState::SUCCESS;

    if (_db.isOpen()) {
        _db.close();
        if (_db.isOpen()) {
            qDebug() << "SQL_DISCONN_FAILURE";
            res = MySQLState::FAILURE;
        } else {
            qDebug() << "SQL_DISCONN_SUCCESS";
            res = MySQLState::SUCCESS;
        }
    } else {
        qDebug() << "SQL_DISCONN_SUCCESS";
        res = MySQLState::SUCCESS;
    }

    return res;
}

void SQLMgr::registerQueryMap()
{
    _queryStr["bloggers_resource"] = QString("SELECT blogger_nickname, blogger_id, blogger_type, "
                                             "blogger_homelink, blogger_fans, blogger_likes, "
                                             "blogger_noteprice, blogger_videoprice, blogger_wechat FROM bloggers_info");

    _queryStr["all_projectList"] = QString("SELECT project_name,project_manager,project_remark,"
                                           "project_update_time FROM project_info WHERE project_manager = '%1' "
                                           "ORDER BY project_update_time DESC");

    _queryStr["bloggers_resource_by_nickname"] = QString("SELECT blogger_nickname, blogger_id, blogger_type, "
                                             "blogger_homelink, blogger_fans, blogger_likes, "
                                             "blogger_noteprice, blogger_videoprice, blogger_wechat FROM bloggers_info "
                                             "WHERE blogger_nickname LIKE '%%1%'");

    _queryStr["bloggers_resource_by_ID"] = QString("SELECT blogger_nickname, blogger_id, blogger_type, "
                                                         "blogger_homelink, blogger_fans, blogger_likes, "
                                                         "blogger_noteprice, blogger_videoprice, blogger_wechat FROM bloggers_info "
                                                         "WHERE blogger_id = '%1'");

    _queryStr["bloggers_resource_by_wechat"] = QString("SELECT blogger_nickname, blogger_id, blogger_type, "
                                                   "blogger_homelink, blogger_fans, blogger_likes, "
                                                   "blogger_noteprice, blogger_videoprice, blogger_wechat FROM bloggers_info "
                                                   "WHERE blogger_wechat = '%1'");
    _queryStr["insert_blogger_to_project"] = QString("INSERT INTO project_note_info (project_id, blogger_id) "
                                                     "SELECT project_id, '%1' FROM project_info "
                                                     "WHERE project_name = '%2';");

    _queryStr["platform_info"] = QString("SELECT platform_name, platform_link FROM platform_info;");

    _queryStr["platform_account_info"] = QString("SELECT pa.platform_account, pa.platform_password "
                                                 "FROM platform_info pi "
                                                 "JOIN platform_account_info pa ON pi.platform_id = pa.platform_id "
                                                 "WHERE pi.platform_name = '%1' AND pa.platform_user = '%2'");

    _queryStr["attachment_upload"] = QString("INSERT INTO attachment (project_id, attachment_file_name, attachment_data) "
                                                 "VALUES (:project_id, :file_name, :file_data);");

    _queryStr["attachment_filename"] = QString("SELECT attachment_file_name FROM attachment "
                                               "WHERE project_id = %1");
}

void SQLMgr::registerTableHeaderMap()
{
    _tableHeader["bloggers_resource"] = QStringList{"blogger_nickname", "blogger_id", "blogger_type", "blogger_homelink",
                                         "blogger_fans", "blogger_likes", "blogger_noteprice",
                                         "blogger_videoprice", "blogger_wechat"};
    _tableHeader["all_projectList"] = QStringList{"项目名称", "项目管理人", "备注", "更新时间"};

    _tableHeader["platform_info"] = QStringList{"平台名称", "平台链接"};

    _tableHeader["platform_account_info"] = QStringList{"账号", "密码"};
}

QString SQLMgr::getQueryStr(const QString& text)
{
    return _queryStr[text];
}

QStringList SQLMgr::getTableHeader(const QString &table)
{
    return _tableHeader[table];
}

QStringList SQLMgr::getAllBloggerTypes()
{
    QStringList bloggerTypes;

    _query.clear();
    _query.prepare("SELECT blogger_type FROM bloggers_info");

    // 执行查询
    if (_query.exec()) {
        while (_query.next()) {
            QString types = _query.value("blogger_type").toString();
            QStringList typesList = types.split("、", Qt::SkipEmptyParts); // 按逗号分割并去掉空项

            for (const QString& type : typesList) {
                if (!bloggerTypes.contains(type.trimmed())) { // 去重处理
                    bloggerTypes.append(type.trimmed());
                }
            }
        }
    } else {
        qDebug() << "Failed to execute query:" << _query.lastError().text();
    }

    return bloggerTypes;
}

QStringList SQLMgr::getAllProjectName()
{
    QStringList projectNames;

    // 假设这里有一个连接到数据库的QSqlDatabase对象，例如_db
    if (_db.isOpen()) {
        _query.clear();
        _query.prepare("SELECT project_name FROM project_info");

        if (_query.exec()) {
            while (_query.next()) {
                QString projectName = _query.value(0).toString();
                projectNames.append(projectName);
            }
        } else {
            qDebug() << "Error executing SQL query:" << _query.lastError().text();
        }
    } else {
        qDebug() << "Database is not open.";
    }

    return projectNames;
}

DBOperation::DBOperationResult SQLMgr::insertBloggersToProject(const QString &project_name, const QString &blogger_id)
{
    if (!_db.isOpen()) {
        qDebug() << "Database is not open";
        return DBOperation::DB_NOT_OPEN;
    }

    _query.clear();
    _query.prepare(getQueryStr("insert_blogger_to_project")
                       .arg(blogger_id).arg(project_name));

    if (!_query.exec()) {
        QSqlError error = _query.lastError();
        if (error.nativeErrorCode() == "1062") {
            qDebug() << "Blogger ID already exists (1062 error code)";
            return DBOperation::DATA_EXIST;
        } else {
            qDebug() << "Failed to add blogger:" << error.text();
            return DBOperation::QUERY_ERR;
        }
    }

    return DBOperation::SUCCESS;
}

DBOperation::DBOperationResult SQLMgr::saveAttachmentToDatabase(const int &project_id, const QString &fileName, QByteArray &fileData)
{
    if (!_db.isOpen()) {
        qDebug() << "Database is not open";
        return DBOperation::DB_NOT_OPEN;
    }

    _query.clear();

    QString queryStr = getQueryStr("attachment_upload");
    _query.prepare(queryStr);

    // 绑定参数
    _query.bindValue(":project_id", project_id);
    _query.bindValue(":file_name", fileName);
    _query.bindValue(":file_data", fileData);

    if (!_query.exec()) {
        QSqlError error = _query.lastError();
        if (error.nativeErrorCode() == "1062") {
            qDebug() << "Blogger ID already exists (1062 error code)";
            return DBOperation::DATA_EXIST;
        } else {
            qDebug() << "Failed to add blogger:" << error.text();
            return DBOperation::QUERY_ERR;
        }
    }

    return DBOperation::SUCCESS;
}

QStringList SQLMgr::getProjectAttachmentList(const int &project_id)
{
    if (!_db.isOpen()) {
        qDebug() << "Database is not open";
        return QStringList();
    }

    QStringList attachmentList;
    QString queryStr = getQueryStr("attachment_filename").arg(project_id);
    _query.clear();
    _query.prepare(queryStr);


    // 执行查询
    if (!_query.exec()) {
        qDebug() << "Failed to execute query:" << _query.lastError().text();
        return attachmentList;
    }

    // 从查询结果中获取文件名并添加到 QStringList 中
    while (_query.next()) {
        QString fileName = _query.value(0).toString(); // 假设文件名在第一列
        attachmentList.append(fileName);
    }

    return attachmentList;
}

QByteArray SQLMgr::getAttachmentData(int project_id, const QString &filename)
{
    QByteArray attachmentData;

    // 构造查询语句
    QString queryStr = "SELECT attachment_data FROM attachment WHERE project_id = :project_id AND attachment_file_name = :filename";

    // 准备查询
    QSqlQuery query(_db);
    query.prepare(queryStr);
    query.bindValue(":project_id", project_id);
    query.bindValue(":filename", filename);

    // 执行查询
    if (query.exec() && query.next()) {
        attachmentData = query.value("attachment_data").toByteArray();
    } else {
        qDebug() << "Failed to retrieve attachment data:" << query.lastError().text();
    }

    return attachmentData;
}

QSqlQueryModel* SQLMgr::queryBloggersInfo() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM bloggers_info");

    if (model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

DBOperation::DBOperationResult SQLMgr::addUser(User user)
{
    if (!_db.isOpen()) {
        qDebug() << "Database is not open";
        return DBOperation::DB_NOT_OPEN;
    }

    _query.clear();
    _query.prepare("INSERT INTO user_info (user_nickname, user_account, user_password, user_level) "
                   "VALUES (:user_nickname, :user_account, :user_password, :user_level)");
    _query.bindValue(":user_nickname", user.name);
    _query.bindValue(":user_account", user.account);
    _query.bindValue(":user_password", user.passwd);
    _query.bindValue(":user_level", user.level);

    if (!_query.exec()) {
        QSqlError error = _query.lastError();
        if (error.nativeErrorCode() == 1062) {
            qDebug() << "User account already exists";
            return DBOperation::ACCOUNT_EXIST;
        } else {
            qDebug() << "Failed to insert user info:" << error.text();
            return DBOperation::QUERY_ERR;
        }
    }

    return DBOperation::SUCCESS;
}

DBOperation::DBOperationResult SQLMgr::varifyLoginInfo(User *user)
{
    if(!_db.isOpen()) {
        qDebug() << "Database is not open";
        return DBOperation::DB_NOT_OPEN;
    }

    QString account = user->account;
    QString passwd = user->passwd;

    _query.clear();
    _query.prepare("SELECT user_nickname, user_password, user_level FROM user_info WHERE user_account = :user_account");
    _query.bindValue(":user_account", account);
    if (!_query.exec()) {
        qDebug() << "Failed to check if user account exists:" << _query.lastError().text();
        return DBOperation::QUERY_ERR;
    }

    if (!_query.next()) {
        qDebug() << "User account does not exist";
        return DBOperation::ACCOUNT_NOT_EXIST;
    }

    QString storedPassword = _query.value("user_password").toString();
    QString storedNickname = _query.value("user_nickname").toString();
    QString storedLevel = _query.value("user_level").toString();

    if (storedPassword != passwd) {
        qDebug() << "Invalid password";
        return DBOperation::INVALID_PASSWORD;
    }

    user->name = storedNickname;
    user->level = storedLevel;

    return DBOperation::SUCCESS;
}

DBOperation::DBOperationResult SQLMgr::createProject(ProjectInfo prjInfo)
{
    if (!_db.isOpen()) {
        qDebug() << "Database is not open";
        return DBOperation::DB_NOT_OPEN;
    }

    _query.clear();
    _query.prepare("INSERT INTO project_info (project_name, project_manager, project_remark, project_update_time) "
                   "VALUES (:project_name, :project_manager, :project_remark, :project_update_time)");
    _query.bindValue(":project_name", prjInfo.project_name);
    _query.bindValue(":project_manager", prjInfo.manager);
    _query.bindValue(":project_remark", prjInfo.remark);
    _query.bindValue(":project_update_time", prjInfo.update_time);

    if (!_query.exec()) {
        QSqlError error = _query.lastError();
        if (error.nativeErrorCode() == "1062") {
            qDebug() << "Project name already exists (1062 error code)";
            return DBOperation::DATA_EXIST;
        } else {
            qDebug() << "Failed to create project:" << error.text();
            return DBOperation::QUERY_ERR;
        }
    }

    return DBOperation::SUCCESS;
}

DBOperation::DBOperationResult SQLMgr::insertBloggersinfo(BloggerInfo bloggerInfo)
{
    if (!_db.isOpen()) {
        qDebug() << "Database is not open";
        return DBOperation::DB_NOT_OPEN;
    }

    _query.clear();
    _query.prepare("INSERT INTO bloggers_info (blogger_nickname, blogger_id, blogger_type, "
                   "blogger_homelink, blogger_fans, blogger_likes, blogger_noteprice, "
                   "blogger_videoprice, blogger_wechat) "
                   "VALUES (:blogger_nickname, :blogger_id, :blogger_type, :blogger_homelink, "
                   ":blogger_fans, :blogger_likes, :blogger_noteprice, :blogger_videoprice, "
                   ":blogger_wechat)");
    _query.bindValue(":blogger_nickname", bloggerInfo.blogger_nickname);
    _query.bindValue(":blogger_id", bloggerInfo.blogger_id);
    _query.bindValue(":blogger_type", bloggerInfo.blogger_type);
    _query.bindValue(":blogger_homelink", bloggerInfo.blogger_homelink);
    _query.bindValue(":blogger_fans", bloggerInfo.blogger_fans);
    _query.bindValue(":blogger_likes", bloggerInfo.blogger_likes);
    _query.bindValue(":blogger_noteprice", bloggerInfo.blogger_noteprice);
    _query.bindValue(":blogger_videoprice", bloggerInfo.blogger_videoprice);
    _query.bindValue(":blogger_wechat", bloggerInfo.blogger_wechat);

    if (!_query.exec()) {
        QSqlError error = _query.lastError();
        if (error.nativeErrorCode() == "1062") {
            qDebug() << "Blogger ID already exists (1062 error code)";
            return DBOperation::DATA_EXIST;
        } else {
            qDebug() << "Failed to add blogger:" << error.text();
            return DBOperation::QUERY_ERR;
        }
    }

    return DBOperation::SUCCESS;
}
