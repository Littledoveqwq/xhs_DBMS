#ifndef WIDGET_PROJECTMANAGE_H
#define WIDGET_PROJECTMANAGE_H

#include <QWidget>

namespace Ui {
class widget_projectManage;
}

class widget_projectManage : public QWidget
{
    Q_OBJECT

public:
    explicit widget_projectManage(QWidget *parent = nullptr);
    ~widget_projectManage();

private:
    Ui::widget_projectManage *ui;
};

#endif // WIDGET_PROJECTMANAGE_H
