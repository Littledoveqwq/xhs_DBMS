#include "linkdelegate.h"
#include <QApplication>
#include "mysqlquerymodel.h"

LinkDelegate::LinkDelegate(QAbstractItemModel *model, QObject *parent)
    : QStyledItemDelegate(parent), model(model) {}

void LinkDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (isLinkColumn(index.column())) {
        painter->save();
        painter->setPen(Qt::blue);

        // 设置字体，并添加下划线
        QFont font = painter->font();
        font.setUnderline(true);
        painter->setFont(font);

        // 获取文本的宽度
        QString text = "链接";
        QFontMetrics metrics(font);
        int textWidth = metrics.horizontalAdvance(text);

        // 计算居中的位置
        int x = option.rect.left() + (option.rect.width() - textWidth) / 2;
        int y = option.rect.top() + (option.rect.height() + metrics.ascent() - metrics.descent()) / 2;

        // 绘制文本
        painter->drawText(x, y, text);
        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool LinkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (isLinkColumn(index.column())) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (option.rect.contains(mouseEvent->pos()) && (mouseEvent->button() == Qt::LeftButton)
                && (mouseEvent->modifiers() & Qt::ControlModifier)) {
                QString url = index.data().toString();
                QDesktopServices::openUrl(QUrl(url));
                return true;
            }
        } else if (event->type() == QEvent::MouseMove || event->type() == QEvent::HoverEnter) {
            if (option.rect.contains(static_cast<QMouseEvent*>(event)->pos())
                && (static_cast<QMouseEvent*>(event)->modifiers() & Qt::ControlModifier)) {
                QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
                event->accept();
            } else {
                QApplication::restoreOverrideCursor();
                event->ignore();
            }
        } else if (event->type() == QEvent::HoverLeave) {
            QApplication::restoreOverrideCursor();
            event->accept();
        } else if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick || event->type() == QEvent::MouseButtonRelease) {
            return false; // Pass the event to the base class
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool LinkDelegate::isLinkColumn(int column) const {
    QString headerText = model->headerData(column, Qt::Horizontal).toString();
    return headerText.contains("链接");
}
