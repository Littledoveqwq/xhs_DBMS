#include "mysqlquerymodel.h"

MySqlQueryModel::MySqlQueryModel(QObject *parent)
    : QSqlQueryModel{parent}
{
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

QVariant MySqlQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QSqlQueryModel::headerData(section, orientation, role);
}

QList<int> MySqlQueryModel::getCheckedRows() const
{
    return checkedRows.values(); // 返回选中行的索引列表
}
