#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>

class MySqlQueryModel : public QSqlQueryModel
{
public:
    explicit MySqlQueryModel(QObject *parent = nullptr);
    void refresh();

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};

#endif // MYSQLQUERYMODEL_H
