#ifndef SQLMGR_H
#define SQLMGR_H
#include "singleton.h"
#include "global.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

class SQLMgr : public QObject, public Singleton<SQLMgr>
{
    Q_OBJECT
public:
    ~SQLMgr();

    /**
     * @brief queryBloggersInfo
     * 查询博主信息,并以 QSqlQueryModel 方式返回
     * @return QSqlQueryModel
     */
    QSqlQueryModel* queryBloggersInfo();
    Register::RegisterResult addUser(User user);
    Login::LoginResult varifyLoginInfo(User* user);

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
     * @return SQLConnState
     * 枚举值
     * SQL_CONN_SUCCESS
     * SQL_CONN_FAILURE
     */
    MySQLState::SQLConnState connectToDB();

    /**
     * @brief disconnectFromDB
     * 断开数据库连接
     * @return SQLConnState
     * 枚举值
     * SQL_CONN_SUCCESS
     * SQL_CONN_FAILURE
     */
    MySQLState::SQLDisconnState disconnectFromDB();

private:
    /**
     * @brief 数据库连接对象
     */
    QSqlDatabase _db;
};

#endif // SQLMGR_H
