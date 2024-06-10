#ifndef PROJECTMANAGEWIDGET_H
#define PROJECTMANAGEWIDGET_H
#include <QTabWidget>
#include <QWidget>
#include "global.h"

namespace Ui {
class ProjectManageWidget;
}

class ProjectManageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectManageWidget(QWidget *parent = nullptr);
    ~ProjectManageWidget();
    void setLabelText(const QString &text);



private slots:


private:
    Ui::ProjectManageWidget *ui;
    MySqlQueryModel* _model;
    int project_id; // 添加 project_id 属性
};

#endif // PROJECTMANAGEWIDGET_H
