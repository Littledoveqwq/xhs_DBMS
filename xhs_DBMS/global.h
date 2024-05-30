#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QDebug>
#include <functional>
#include <memory>
#include <iostream>
#include <mutex>

/**
 * @brief sql_conn_state
 * 表示数据库是否连接成功
 */
typedef enum {
    SQL_CONN_SUCCESS = 0,       // 连接成功
    SQL_CONN_FAILURE = 1,       // 连接失败
    SQL_DISCONN_SUCCESS = 2,    // 断开连接成功
    SQL_DISCONN_FAILURE = 3     // 断开连接失败
}sql_conn_state;



#endif // GLOBAL_H
