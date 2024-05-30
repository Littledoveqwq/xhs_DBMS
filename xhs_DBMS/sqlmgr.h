#ifndef SQLMGR_H
#define SQLMGR_H
#include "singleton.h"
#include "global.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class SQLMgr : public QObject, public Singleton<SQLMgr>
{
    Q_OBJECT
public:
    ~SQLMgr();
private:
    /**
     * 为了能让基类 Singleton<SQLMgr> 访问构造函数
     * 声明基类为这个类的友元
     */
    friend class Singleton<SQLMgr>;
    SQLMgr();
    /**
     * @brief connectToDB
     * 连接数据库函数
     * @return sql_conn_state
     * 枚举值
     * SQL_CONN_SUCCESS
     * SQL_CONN_FAILURE
     */
    sql_conn_state connectToDB();
    sql_conn_state disconnectFromDB();

private:
    /**
     * @brief 数据库连接对象
     */
    QSqlDatabase _db;
};

#endif // SQLMGR_H
