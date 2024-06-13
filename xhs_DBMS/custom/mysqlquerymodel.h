#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSet>
#include <QDebug>
#include <QSqlRecord>      // 包含 QSqlRecord 的头文件
#include <QStringList>

#define CHECK_BOX_COLUMN 0

class MySqlQueryModel : public QSqlQueryModel
{
public:
    explicit MySqlQueryModel(QString table_name, QObject *parent = nullptr);
    void refresh();

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList getHeader();

private:
    void headerDataFromDB() const;

private:
    QSet<int> checkedRows;  // 用于存储选中的行
    QString _queryStr; // Store the query string
    QString _table_name;
    mutable QStringList _columns; // 保存列名的数组
    void getColumns(const QString &newTable_name);
};

#endif // MYSQLQUERYMODEL_H
