#ifndef PROJECTMANAGEWIDGET_H
#define PROJECTMANAGEWIDGET_H
#include <QTabWidget>
#include <QWidget>
#include "global.h"
#include <QMenu>

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
    void getProjectId(const QString &text);

private slots:

    void on_btn_projectTeamer_released();

private:
    Ui::ProjectManageWidget *ui;
    MySqlQueryModel* _model;
    int _project_id; // 添加 project_id 属性
    QMenu *m_menu;
};

#endif // PROJECTMANAGEWIDGET_H
