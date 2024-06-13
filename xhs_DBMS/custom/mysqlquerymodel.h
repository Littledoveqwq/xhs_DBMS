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
    Q_OBJECT
public:
    explicit MySqlQueryModel(QObject *parent = nullptr);
    void refresh();

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QSet<int> checkedRows;  // 用于存储选中的行

};

#endif // MYSQLQUERYMODEL_H
