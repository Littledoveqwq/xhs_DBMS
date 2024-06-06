#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

/******************************************************************************
 *
 * @file       registerdialog.h
 * @brief      注册界面
 *
 * @author     sheep
 * @date       2024/06/01
 * @history
 *****************************************************************************/
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_confirm_btn_clicked();

    void on_cancel_btn_clicked();

signals:
    void sig_cancel_reg();

private:
    void showTip(QString text, bool b_ok);
    void initHttpHandlers();
private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
