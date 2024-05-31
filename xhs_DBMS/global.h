#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QDebug>
#include <functional>
#include <memory>
#include <iostream>
#include <mutex>

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





#endif // GLOBAL_H
