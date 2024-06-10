#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QCheckBox>
#include <QMouseEvent>
#include <QApplication>

class CheckBoxDelegate : public QStyledItemDelegate {
public:
    CheckBoxDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() == 0) {
            QStyleOptionButton opt;
            opt.rect = option.rect;
            opt.state |= QStyle::State_Enabled | QStyle::State_Active;
            if (index.data(Qt::CheckStateRole).toBool()) {
                opt.state |= QStyle::State_On;
            } else {
                opt.state |= QStyle::State_Off;
            }
            QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override {
        if (index.column() == 0) {
            if (event->type() == QEvent::MouseButtonRelease) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                if (option.rect.contains(mouseEvent->pos())) {
                    // 用户点击了复选框区域
                    // 修改数据模型中的数据以反映用户的选择
                    QVariant currentState = model->data(index, Qt::CheckStateRole);
                    Qt::CheckState newState = (currentState == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
                    model->setData(index, newState, Qt::CheckStateRole);
                }
            }
            return true; // 返回 true 表示事件已被处理
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
};

#endif // CHECKBOXDELEGATE_H
