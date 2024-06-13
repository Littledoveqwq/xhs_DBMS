#ifndef SQLMGR_H
#define SQLMGR_H
#include "singleton.h"
#include "global.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMap>

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

    /**
     * @brief addUser
     * 添加用户
     * @param user
     * @return
     */
    DBOperation::DBOperationResult addUser(User user);

    /**
     * @brief varifyLoginInfo
     * 验证登录信息:账号, 密码
     * @param user
     * @return
     */
    DBOperation::DBOperationResult varifyLoginInfo(User* user);

    DBOperation::DBOperationResult createProject(ProjectInfo prjInfo);

    DBOperation::DBOperationResult insertBloggersinfo(BloggerInfo bloggerInfo);

    QString getQueryStr(const QString& table);
    QStringList getTableHeader(const QString& table);

    QStringList getAllBloggerTypes();
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
    MySQLState::SQLConnState disconnectFromDB();

    void registerQueryMap();
    void registerTableHeaderMap();

private:
    /**
     * @brief 数据库连接对象
     */
    QSqlDatabase _db;
    QSqlQuery _query;
    QMap<QString, QString> _queryStr;
    QMap<QString, QStringList> _tableHeader;
};

#endif // SQLMGR_H
