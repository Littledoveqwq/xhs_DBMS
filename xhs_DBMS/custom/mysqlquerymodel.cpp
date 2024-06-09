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
    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    } else {
        return QSqlQueryModel::data(index, role);
    }
}
