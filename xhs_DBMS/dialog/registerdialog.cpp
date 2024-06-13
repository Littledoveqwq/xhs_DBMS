#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "sqlmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->pass_edit->setToolTip("密码必须包含英文和数字, 长度 8 ~ 16 位");
    //将err_tip的state属性设置成normal, 配合qss使用
    ui->err_tip->setProperty("state","normal");
    //调用全局函数刷新err_tip
    repolish(ui->err_tip);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_confirm_btn_clicked()
{
    if(ui->name_edit->text().trimmed().isEmpty()) {
        showTip(tr("昵称不能为空"), false);
        return;
    }

    if(ui->account_edit->text().trimmed().isEmpty()) {
        showTip(tr("用户名不能为空"), false);
        return;
    }

    if(ui->pass_edit->text().trimmed().isEmpty()) {
        showTip(tr("密码不能为空"), false);
        return;
    }

    if(ui->confirm_edit->text().trimmed().isEmpty()) {
        showTip(tr("确认密码不能为空"), false);
        return;
    }

    auto account = ui->account_edit->text();
    //声明称静态的,这样只有第一次调用才会生成
    static const QRegularExpression userRegex("^(?=.*[A-Za-z])[A-Za-z0-9_]*$");    //只能输入英文数字和下划线
    bool match = userRegex.match(account).hasMatch();

    if(!match) {
        showTip(tr("用户名格式有误"), false);
        return;
    }

    auto pass = ui->pass_edit->text();
    static const QRegularExpression passRegex("^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d\\W]{8,16}$");    //必须包含数字和英文
    match = passRegex.match(pass).hasMatch();

    if(!match) {
        showTip(tr("密码格式有误"), false);
        return;
    }

    if(ui->pass_edit->text() != ui->confirm_edit->text()) {
        showTip(tr("两次输入的密码不一致"), false);
        return;
    }

    QString name = ui->name_edit->text();
    account = ui->account_edit->text();
    pass = ui->pass_edit->text();
    QByteArray passwordBytes = pass.toUtf8();
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(passwordBytes);
    QByteArray hashedBytes = hash.result();
    pass = hashedBytes.toHex();

    User user{name, account, pass, "user"};

    DBOperation::DBOperationResult res = SQLMgr::getInstance()->addUser(user);

    if(res == DBOperation::DBOperationResult::SUCCESS) {
        showTip(tr("注册成功"), true);
    } else if(res == DBOperation::DBOperationResult::ACCOUNT_EXIST){
        showTip(tr("该用户名已被注册"), false);
    } else {
        showTip(tr("注册失败"), false);
    }
}

void RegisterDialog::showTip(QString text, bool b_ok)
{
    ui->err_tip->setText(text);
    ui->err_tip->adjustSize();
    if(b_ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }
    qDebug() << ui->err_tip->property("state").toString();
    repolish(ui->err_tip);
}


void RegisterDialog::on_cancel_btn_clicked()
{
    emit sig_cancel_reg();
}

