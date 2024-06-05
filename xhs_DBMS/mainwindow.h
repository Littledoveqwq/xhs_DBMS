#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog/logindialog.h"
#include "dialog/registerdialog.h"
#include "appbasewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    LoginDialog* _login_dlg;
    RegisterDialog* _reg_dlg;
    AppBaseWindow* _app_basewindow;
public slots:
    void slot_switch_reg();

    void slot_switch_login();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
