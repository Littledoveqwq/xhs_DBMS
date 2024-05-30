#include "sqlmgr.h"

SQLMgr::~SQLMgr() {
    qDebug() << "SQLMgr destruct";
    disconnectFromDB();
}

SQLMgr::SQLMgr() {
    connectToDB();
}

sql_conn_state SQLMgr::connectToDB() {
    sql_conn_state res = SQL_CONN_SUCCESS;
    _db = QSqlDatabase::addDatabase("QMYSQL");
    _db.setHostName("106.14.63.204");
    _db.setPort(3306);
    _db.setDatabaseName("bloggersprojectmanagement");
    _db.setUserName("root");
    _db.setPassword("123456");

    bool ok = _db.open();
    if(ok) {
        qDebug() << "SQL_CONN_SUCCESS";
    } else {
        qDebug() << "SQL_CONN_FAILURE: " << _db.lastError();
        res = SQL_CONN_FAILURE;
    }

    return res;
}

sql_conn_state SQLMgr::disconnectFromDB() {
    sql_conn_state res = SQL_DISCONN_SUCCESS;

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
