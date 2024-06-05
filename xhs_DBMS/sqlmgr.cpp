#include "sqlmgr.h"
#include "configmgr.h"

SQLMgr::~SQLMgr() {
    disconnectFromDB();
}

SQLMgr::SQLMgr() {
    connectToDB();
}

MySQLState::SQLConnState SQLMgr::connectToDB() {
    MySQLState::SQLConnState res = MySQLState::SQL_CONN_SUCCESS;

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
    } else {
        qDebug() << "SQL_CONN_FAILURE: " << _db.lastError();
        res = MySQLState::SQL_CONN_FAILURE;
    }

    return res;
}

MySQLState::SQLDisconnState SQLMgr::disconnectFromDB() {
    MySQLState::SQLDisconnState res = MySQLState::SQL_DISCONN_SUCCESS;

    if (_db.isOpen()) {
        _db.close();
        if (_db.isOpen()) {
            // 如果数据库仍然打开，表示断开连接失败
            qDebug() << "SQL_DISCONN_FAILURE";
            res = MySQLState::SQL_DISCONN_FAILURE;
        } else {
            qDebug() << "SQL_DISCONN_SUCCESS";
            res = MySQLState::SQL_DISCONN_SUCCESS;
        }
    } else {
        // 数据库本来就没有打开，也算成功
        qDebug() << "SQL_DISCONN_SUCCESS";
        res = MySQLState::SQL_DISCONN_SUCCESS;
    }

    return res;
}

QSqlQueryModel* SQLMgr::queryBloggersInfo() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM bloggers_info");

    // 检查是否查询成功
    if (model->lastError().isValid()) {
        qDebug() << "Failed to execute query:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

Register::RegisterResult SQLMgr::addUser(User user)
{
    if(!_db.isOpen()) {
        qDebug() << "database is not open";
        return Register::DB_NOT_OPEN;
    }

    // 检查用户名是否已经存在
    QSqlQuery checkQuery(_db);
    checkQuery.prepare("SELECT COUNT(*) FROM user_info WHERE user_account = :user_account");
    checkQuery.bindValue(":user_account", user.account);
    if (!checkQuery.exec()) {
        qDebug() << "Failed to check if useraccount already exists:" << checkQuery.lastError().text();
        return Register::QUERY_ERR;
    }

    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        qDebug() << "Useraccount already exists";
        return Register::ACCOUNT_EXIST;
    }

    // 插入用户信息
    QSqlQuery insertQuery(_db);
    insertQuery.prepare("INSERT INTO user_info(user_nickname, user_account, user_password, user_level) "
                        "VALUES (:user_nickname, :user_account, :user_password, :user_level)");
    insertQuery.bindValue(":user_nickname", user.name);
    insertQuery.bindValue(":user_account", user.account);
    insertQuery.bindValue(":user_password", user.passwd);
    insertQuery.bindValue(":user_level", user.level);

    if (!insertQuery.exec()) {
        qDebug() << "Failed to insert user info:" << insertQuery.lastError().text();
        return Register::QUERY_ERR;
    }

    return Register::SUCCESS;
}

Login::LoginResult SQLMgr::varifyLoginInfo(User *user)
{
    if(!_db.isOpen()) {
        qDebug() << "database is not open";
        return Login::DB_NOT_OPEN;
    }

    QString account = user->account;
    QString passwd = user->passwd;

    // 检查用户名是否存在
    QSqlQuery query(_db);
    query.prepare("SELECT user_nickname, user_password, user_level FROM user_info WHERE user_account = :user_account");
    query.bindValue(":user_account", account);
    if (!query.exec()) {
        qDebug() << "Failed to check if user account exists:" << query.lastError().text();
        return Login::QUERY_ERR;
    }

    if (!query.next()) {
        qDebug() << "User account does not exist";
        return Login::ACCOUNT_NOTEXIST;
    }

    // 获取用户信息
    QString storedPassword = query.value("user_password").toString();
    QString storedNickname = query.value("user_nickname").toString();
    QString storedLevel = query.value("user_level").toString();

    // 检查密码是否匹配
    if (storedPassword != passwd) {
        qDebug() << "Invalid password";
        return Login::INVALID_PASSWORD;
    }

    // 填充用户信息
    user->name = storedNickname;
    user->level = storedLevel;

    return Login::SUCCESS;
}


