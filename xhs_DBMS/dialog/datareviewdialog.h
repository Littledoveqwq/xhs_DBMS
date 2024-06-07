#ifndef DATAREVIEWDIALOG_H
#define DATAREVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class datareviewdialog;
}

class datareviewdialog : public QDialog
{
    Q_OBJECT

public:
    explicit datareviewdialog(QWidget *parent = nullptr);
    ~datareviewdialog();

private:
    Ui::datareviewdialog *ui;
};

#endif // DATAREVIEWDIALOG_H
