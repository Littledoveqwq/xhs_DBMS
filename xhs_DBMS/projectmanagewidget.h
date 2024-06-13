#ifndef PROJECTMANAGEWIDGET_H
#define PROJECTMANAGEWIDGET_H
#include <QTabWidget>
#include <QWidget>
#include "global.h"
#include <QMenu>
#include <QFileDialog>

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

    void on_btn_bfUpload_clicked();

    void on_btn_bfDownload_clicked();

private:
    Ui::ProjectManageWidget *ui;
    MySqlQueryModel* _model;
    int _project_id; // 添加 project_id 属性
    QMenu *m_menu;

    void downloadAttachment(const QString& filename);
};

#endif // PROJECTMANAGEWIDGET_H
