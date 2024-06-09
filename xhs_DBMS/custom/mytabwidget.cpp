#include "mytabwidget.h"
#include <QDebug>

MyTabWidget::MyTabWidget(QWidget *parent)
    : QTabWidget(parent)
{

}
void MyTabWidget::closeEvent(QCloseEvent *event)
{
    emit sig_tabWidget_close();
    qDebug() << "tabwidget close";
    QTabWidget::closeEvent(event);
}

void MyTabWidget::removeAllTabs() {
    while (this->count() > 0) {
        QWidget* widget = this->widget(0); // 获取第一个选项卡的 widget
        this->removeTab(0); // 从 QTabWidget 中移除选项卡
        delete widget; // 删除 widget
    }
}
