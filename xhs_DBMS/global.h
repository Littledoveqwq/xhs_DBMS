#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QStyle>
#include <QDebug>
#include <functional>
#include <memory>
#include <iostream>
#include <mutex>
#include <QRegularExpression>
#include <QCryptographicHash>  //md5加密的库
#include <QByteArray>
#include <QDateTime>
#include <QApplication>
#include <QScreen>

#include "custom/mysqlquerymodel.h"
#include "custom/linkdelegate.h"
#include "custom/mytabwidget.h"

namespace MySQLState
{
/**
* @brief SQLConnState
* 表示数据库是否连接成功
*/
typedef enum {
    SQL_CONN_SUCCESS = 0,
    SQL_CONN_FAILURE = 1
} SQLConnState;

/**
* @brief SQLDisconnState
* 表示数据库是否断开成功
*/
typedef enum {
    SQL_DISCONN_SUCCESS = 0,
    SQL_DISCONN_FAILURE = 1
} SQLDisconnState;

/**
* @brief SQLQueryResult
* 表示数据库是否查询成功
*/
typedef enum {
    SQL_QUERY_SUCCESS = 0,
    SQL_QUERY_FAILURE = 1
}SQLQueryResult;
}

namespace Register
{
/**
* @brief RegisterResult
* 注册结果
*/
typedef enum {
    SUCCESS = 0,
    ACCOUNT_EXIST = 1,
    QUERY_ERR = 2,
    DB_NOT_OPEN = 3,
}RegisterResult;
}

namespace Login
{
/**
* @brief LoginResult
* 登录结果
*/
typedef enum {
    SUCCESS = 0,
    ACCOUNT_NOTEXIST = 1,
    INVALID_PASSWORD = 2,   //密码不匹配
    QUERY_ERR = 3,
    DB_NOT_OPEN = 4,
}LoginResult;
}

namespace InsertData {
typedef enum {
    SUCCESS = 0,
    DB_NOT_OPEN = 1,
    QUERY_ERR = 2,
    DATA_EXIST = 3,
    MANAGER_NOT_EXIST = 4,
}InsertResult;
}

/**
 * @brief repolish
 * 用于刷新控件
 */
extern std::function<void(QWidget*)> repolish;

extern std::function<void(QWidget*)> moveToCenter;

extern std::function<void(MySqlQueryModel*)> updateHeadertoChinese;

extern QMap<QString, QString> columnMapping;

/**
 * @brief MySQLInfo
 * 用于从配置文件中读取信息
 */
typedef struct _MySQLInfo {
    QString host;
    int port;
    QString database_name;
    QString user_name;
    QString passwd;
}MySQLInfo;

typedef struct _User {
    QString name;
    QString account;
    QString passwd;
    QString level;
}User;

typedef struct _ProjectInfo{
    QString project_name;
    QString manager;
    QString remark;
    QString update_time;
}ProjectInfo;

typedef struct _BloggerInfo{
    QString blogger_nickname;
    QString blogger_id;
    QString blogger_type;
    QString blogger_homelink;
    int blogger_fans;
    int blogger_likes;
    int blogger_noteprice;
    int blogger_videoprice;
    QString blogger_wechat;
}BloggerInfo;
#endif // GLOBAL_H

