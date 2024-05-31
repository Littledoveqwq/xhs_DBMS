#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "dialog/setProjectinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    /**
     * @brief initComboBox
     * 初始化 ComboBox 使其样式生效
     */
    void initComboBox();

private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_rbtn_exactSearch_clicked();

    void on_rbtn_fuzzySearch_clicked();

    void on_btn_projectNew_clicked();

    void open_dialog_setProjectinfo();

    void on_rbt_recent_clicked();

    void on_rbt_favour_clicked();

private:
    Ui::Widget *ui;

    setProjectinfo* projectinfo;
};
#endif // WIDGET_H
