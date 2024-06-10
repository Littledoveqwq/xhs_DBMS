#ifndef BLOGGERREVISEDIALOG_H
#define BLOGGERREVISEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class BloggerReviseDialog;
}

class BloggerReviseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BloggerReviseDialog(QWidget *parent = nullptr);
    ~BloggerReviseDialog();

    void setInfo(BloggerInfo bloggerInfo);

private slots:
    void on_btn_submit_clicked();

    void on_btn_delete_clicked();

private:
    Ui::BloggerReviseDialog *ui;

    BloggerInfo* _blogger;
};

#endif // BLOGGERREVISEDIALOG_H
