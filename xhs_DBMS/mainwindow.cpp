#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowTitle("登录");

    // 初始化登录对话框
    _login_dlg = new LoginDialog(this);
    connect(_login_dlg, &LoginDialog::sig_switch_reg,
            this, &MainWindow::slot_switch_reg);
    connect(_login_dlg, &LoginDialog::sig_login_success,
            this, &MainWindow::slot_switch_basewindow);

    setCentralWidget(_login_dlg);
    setMinimumSize(_login_dlg->size());
    setMaximumSize(_login_dlg->size());
}

MainWindow::~MainWindow()
{
    // 使用 deleteLater() 延迟删除对象
    if(_reg_dlg) _reg_dlg->deleteLater();
    if(_login_dlg) _login_dlg->deleteLater();
    if(_app_basewindow) _app_basewindow->deleteLater();
    delete ui;
}

void MainWindow::slot_switch_reg()
{
    setWindowTitle("注册");

    // 仅在 _reg_dlg 为空时创建新的注册对话框
    _reg_dlg = new RegisterDialog(this);
    connect(_reg_dlg, &RegisterDialog::sig_cancel_reg,
            this, &MainWindow::slot_switch_login);

    // 切换到注册对话框
    setCentralWidget(_reg_dlg);
    setMinimumSize(_reg_dlg->size());
    setMaximumSize(_reg_dlg->size());

    if (_login_dlg) {
        _login_dlg->hide();
    }
    _reg_dlg->show();
}

void MainWindow::slot_switch_login()
{
    setWindowTitle("登录");

    // 仅在 _login_dlg 为空时创建新的登录对话框
    _login_dlg = new LoginDialog(this);
    connect(_login_dlg, &LoginDialog::sig_switch_reg,
            this, &MainWindow::slot_switch_reg);
    connect(_login_dlg, &LoginDialog::sig_login_success,
            this, &MainWindow::slot_switch_basewindow);

    // 切换到登录对话框
    setCentralWidget(_login_dlg);
    setMinimumSize(_login_dlg->minimumSize());
    setMaximumSize(_login_dlg->maximumSize());

    if (_app_basewindow) {
        _app_basewindow->hide();
        _app_basewindow->deleteLater();
        _app_basewindow = nullptr;
    }
    if (_reg_dlg) {
        _reg_dlg->hide();
    }

    _login_dlg->show();
    moveToCenter(this);
}

void MainWindow::slot_switch_basewindow(User user)
{
    setWindowTitle("小红书博主管理系统");

    if (!_app_basewindow) {
        // 仅在 _app_basewindow 为空时创建新的主窗口
        _app_basewindow = new AppBaseWindow(this);
        connect(_app_basewindow, &AppBaseWindow::sig_switch_login,
                this, &MainWindow::slot_switch_login);
    }

    _user = new User{user.name, user.account, user.passwd, user.level};
    _app_basewindow->setUserName(_user->name);

    setCentralWidget(_app_basewindow);
    setMinimumSize(_app_basewindow->minimumSize());
    setMaximumSize(_app_basewindow->maximumSize());

    if (_login_dlg) {
        _login_dlg->hide();
    }

    _app_basewindow->show();
    moveToCenter(this);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_app_basewindow) {
        _app_basewindow->close();
    }
    QMainWindow::closeEvent(event);
}
