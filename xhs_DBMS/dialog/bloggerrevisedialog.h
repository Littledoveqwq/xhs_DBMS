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

private:
    Ui::BloggerReviseDialog *ui;
};

#endif // BLOGGERREVISEDIALOG_H
