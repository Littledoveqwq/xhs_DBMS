#ifndef LINKDELEGATE_H
#define LINKDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QAbstractItemModel>

class LinkDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    LinkDelegate(QAbstractItemModel *model, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    bool isLinkColumn(int column) const;

    QAbstractItemModel *model;
};

#endif // LINKDELEGATE_H
