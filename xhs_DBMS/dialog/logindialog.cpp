#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
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

}

