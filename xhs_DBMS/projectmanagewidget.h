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


private:
    Ui::ProjectManageWidget *ui;
    MySqlQueryModel* _model;
};

#endif // PROJECTMANAGEWIDGET_H
