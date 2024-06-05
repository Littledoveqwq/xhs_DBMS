#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowTitle("登录");
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
    delete ui;
}

void MainWindow::slot_switch_reg()
{
    setWindowTitle("注册");
    //setCentralWidget 会回收被切换的界面, 所以要重新new
    //Note QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time
    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->hide();

    connect(_reg_dlg, &RegisterDialog::sig_cancel_reg,
            this, &MainWindow::slot_switch_login);

    setCentralWidget(_reg_dlg);
    setMinimumSize(_reg_dlg->size());
    setMaximumSize(_reg_dlg->size());

    _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::slot_switch_login()
{
    setWindowTitle("登录");
    _login_dlg = new LoginDialog(this);
    connect(_login_dlg, &LoginDialog::sig_switch_reg,
            this, &MainWindow::slot_switch_reg);

    connect(_login_dlg, &LoginDialog::sig_login_success,
            this, &MainWindow::slot_switch_basewindow);

    setCentralWidget(_login_dlg);
    setMinimumSize(_login_dlg->minimumSize());
    setMaximumSize(_login_dlg->maximumSize());

    _reg_dlg->hide();
    _login_dlg->show();
}

void MainWindow::slot_switch_basewindow(User user)
{
    setWindowTitle("小红书博主管理系统");
    _user = new User{user.name, user.account, user.passwd, user.level};
    _app_basewindow = new AppBaseWindow(this);
    _app_basewindow->setUserName(_user->name);

    setCentralWidget(_app_basewindow);
    setMinimumSize(_app_basewindow->minimumSize());
    setMaximumSize(_app_basewindow->maximumSize());

    _login_dlg->hide();
    _app_basewindow->show();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->geometry();

    int x = (screenRect.width() - this->width()) / 2;
    int y = (screenRect.height() - this->height()) / 2;
    this->move(x, y);
}
