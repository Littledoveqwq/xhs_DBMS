#include "mysqlquerymodel.h"

MySqlQueryModel::MySqlQueryModel(QString table_name, QObject *parent)
    : QSqlQueryModel{parent}
{
    getColumns(table_name);
    _columns.push_front("");
}

void MySqlQueryModel::refresh()
{
    setQuery(query().executedQuery());
}

QVariant MySqlQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // 设置文本对齐方式
    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    // 处理复选框列的数据
    if (index.column() == CHECK_BOX_COLUMN) {
        if (role == Qt::CheckStateRole) {
            return checkedRows.contains(index.row()) ? Qt::Checked : Qt::Unchecked;
        }
        return QVariant();
    }

    // 调整列索引以适应复选框列
    QModelIndex sqlIndex = this->index(index.row(), index.column() - 1);

    // 返回查询结果列的数据
    return QSqlQueryModel::data(sqlIndex, role);
}

int MySqlQueryModel::columnCount(const QModelIndex &parent) const
{
    // 增加一列用于复选框
    return QSqlQueryModel::columnCount(parent) + 1;
}

bool MySqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    // 处理复选框列的数据设置
    if (index.column() == CHECK_BOX_COLUMN && role == Qt::CheckStateRole) {
        if (value == Qt::Checked) {
            checkedRows.insert(index.row());
        } else {
            checkedRows.remove(index.row());
        }
        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    }

    // 调整列索引以适应复选框列
    QModelIndex sqlIndex = this->index(index.row(), index.column() - 1);

    return QSqlQueryModel::setData(sqlIndex, value, role);
}

Qt::ItemFlags MySqlQueryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == CHECK_BOX_COLUMN)
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;

    return QSqlQueryModel::flags(index);
}

void MySqlQueryModel::headerDataFromDB() const {
    // 构建查询字符串并执行查询
    QString queryString = QString("SELECT column_name FROM information_schema.columns WHERE table_name = '%1'")
                              .arg(_table_name);
    QSqlQuery query(queryString);

    if (query.exec()) {
        _columns.clear(); // 清空已有的列名

        while (query.next()) {
            QString columnName = query.value("column_name").toString();
            _columns.append(columnName);
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
        // 可以添加适当的错误处理逻辑
    }
}

void MySqlQueryModel::getColumns(const QString &newTable_name)
{
    _table_name = newTable_name;
    headerDataFromDB();
}

QStringList MySqlQueryModel::getHeader()
{
    return _columns;
}
