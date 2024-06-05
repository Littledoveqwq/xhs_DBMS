#include "sqlmgr.h"
#include "configmgr.h"

SQLMgr::~SQLMgr() {
    disconnectFromDB();
}

SQLMgr::SQLMgr() {
    connectToDB();
}

SQLConnState SQLMgr::connectToDB() {
    SQLConnState res = SQL_CONN_SUCCESS;

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
        res = SQL_CONN_FAILURE;
    }

    return res;
}

SQLDisconnState SQLMgr::disconnectFromDB() {
    SQLDisconnState res = SQL_DISCONN_SUCCESS;

    if (_db.isOpen()) {
        _db.close();
        if (_db.isOpen()) {
            // 如果数据库仍然打开，表示断开连接失败
            qDebug() << "SQL_DISCONN_FAILURE";
            res = SQL_DISCONN_FAILURE;
        } else {
            qDebug() << "SQL_DISCONN_SUCCESS";
            res = SQL_DISCONN_SUCCESS;
        }
    } else {
        // 数据库本来就没有打开，也算成功
        qDebug() << "SQL_DISCONN_SUCCESS";
        res = SQL_DISCONN_SUCCESS;
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

RegisterResult SQLMgr::addUser(User user)
{
    if(!_db.isOpen()) {
        qDebug() << "database is not open";
        return REGISTER_FAILURE;
    }

    // 检查用户名是否已经存在
    QSqlQuery checkQuery(_db);
    checkQuery.prepare("SELECT COUNT(*) FROM user_info WHERE user_nickname = :user_nickname");
    checkQuery.bindValue(":user_nickname", user.name);
    if (!checkQuery.exec()) {
        qDebug() << "Failed to check if username already exists:" << checkQuery.lastError().text();
        return REGISTER_FAILURE;
    }

    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        qDebug() << "Username already exists";
        return ACCOUNT_EXIST;
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
        return REGISTER_FAILURE;
    }

    return REGISTER_SUCCESS;
}


