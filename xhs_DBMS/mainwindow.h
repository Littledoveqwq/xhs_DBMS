#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog/logindialog.h"
#include "dialog/registerdialog.h"
#include "appbasewindow.h"
#include <QScreen>

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

public slots:
    void slot_switch_reg();

    void slot_switch_login();

    void slot_switch_basewindow(User user);
private:
    Ui::MainWindow *ui;
    LoginDialog* _login_dlg{nullptr};
    RegisterDialog* _reg_dlg{nullptr};
    AppBaseWindow* _app_basewindow{nullptr};
    User* _user;



    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
