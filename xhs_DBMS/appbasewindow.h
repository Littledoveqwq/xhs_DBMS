#ifndef APPBASEWINDOW_H
#define APPBASEWINDOW_H

#include <QWidget>
#include "dialog/prjinfodialog.h"
#include <QSqlQueryModel>



QT_BEGIN_NAMESPACE
namespace Ui {
class AppBaseWindow;
}
QT_END_NAMESPACE

class AppBaseWindow : public QWidget
{
    Q_OBJECT

public:
    AppBaseWindow(QWidget *parent = nullptr);
    ~AppBaseWindow();
    void setUserName(const QString& username);


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
    Ui::AppBaseWindow *ui;

    PrjInfoDialog* projectinfo;
};
#endif // APPBASEWINDOW_H
