#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
/******************************************************************************
 *
 * @file       logindialog.h
 * @brief      登录界面
 *
 * @author     sheep
 * @date       2024/06/01
 * @history
 *****************************************************************************/
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void sig_switch_reg();
    void sig_login_success(User user);

private slots:
    void on_login_btn_clicked();

    /**
     * @brief on_btn_forget_clicked
     * 忘记密码
     */
    void on_btn_forget_clicked();

private:
    Ui::LoginDialog *ui;
    void showTip(QString text, bool b_ok);
};

#endif // LOGINDIALOG_H
