#include "logindialog.h"
#include "ui_logindialog.h"
#include "global.h"
#include "sqlmgr.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    //将err_tip的state属性设置成normal, 配合qss使用
    ui->err_tip->setProperty("state","normal");
    //调用全局函数刷新err_tip
    repolish(ui->err_tip);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    //连接注册按钮, 按钮点击则发送 switchRegister 信号, mainWindow 接收, 调用 mainWindow 的 slot 函数
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::sig_switch_reg);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_login_btn_clicked()
{
    ui->account_edit->setText("Littledoveqwq");
    ui->pass_edit->setText("djw3224171");

    if(ui->account_edit->text().trimmed().isEmpty()) {
        showTip("用户名不能为空", false);
        return;
    }

    if(ui->pass_edit->text().trimmed().isEmpty()) {
        showTip("密码不能为空", false);
        return;
    }

    User* user = new User;
    auto account = ui->account_edit->text();
    auto pass = ui->pass_edit->text();
    QByteArray passwordBytes = pass.toUtf8();
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(passwordBytes);
    QByteArray hashedBytes = hash.result();
    pass = hashedBytes.toHex();

    user->passwd = pass;
    user->account = account;

    DBOperation::DBOperationResult res = SQLMgr::getInstance()->varifyLoginInfo(user);
    if(res == DBOperation::DBOperationResult::ACCOUNT_NOT_EXIST) {
        showTip("用户名不存在", false);
        return;
    } else if (res == DBOperation::DBOperationResult::INVALID_PASSWORD) {
        showTip("密码错误", false);
        return;
    } else if (res == DBOperation::DBOperationResult::DB_NOT_OPEN || res == DBOperation::DBOperationResult::QUERY_ERR) {
        showTip("登录失败", false);
        return;
    }

    if(res == DBOperation::DBOperationResult::SUCCESS) {
        showTip("登录成功", true);
        emit sig_login_success(*user);
        return;
    }
}

void LoginDialog::showTip(QString text, bool b_ok)
{
    ui->err_tip->setText(text);
    ui->err_tip->adjustSize();
    if(b_ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }

    repolish(ui->err_tip);
}

void LoginDialog::on_btn_forget_clicked()
{

}

