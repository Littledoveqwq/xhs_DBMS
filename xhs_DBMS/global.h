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

typedef enum {
    REGISTER_SUCCESS = 0,
    ACCOUNT_EXIST = 1,
    REGISTER_FAILURE = 2,
}RegisterResult;

/**
 * @brief repolish
 * 用于刷新控件
 */
extern std::function<void(QWidget*)> repolish;

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

#endif // GLOBAL_H
