#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _login_dlg = new LoginDialog(this);
    connect(_login_dlg, &LoginDialog::sig_switch_reg,
            this, &MainWindow::slot_switch_reg);



    _app_basewindow = new AppBaseWindow(this);
    _app_basewindow->hide();

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
    //setCentralWidget 会回收被切换的界面, 所以要重新new
    //Note QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time
    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->hide();

    connect(_reg_dlg, &RegisterDialog::sig_cancel_reg,
            this, &MainWindow::slot_switch_login);

    setCentralWidget(_reg_dlg);
    setMinimumSize(_reg_dlg->minimumSize());
    setMaximumSize(_reg_dlg->maximumSize());

    _login_dlg->hide();
    _reg_dlg->show();
}

void MainWindow::slot_switch_login()
{
    _login_dlg = new LoginDialog(this);
    connect(_login_dlg, &LoginDialog::sig_switch_reg,
            this, &MainWindow::slot_switch_reg);

    setCentralWidget(_login_dlg);
    setMinimumSize(_login_dlg->minimumSize());
    setMaximumSize(_login_dlg->maximumSize());

    _reg_dlg->hide();
    _login_dlg->show();
}
