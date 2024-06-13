#ifndef APPBASEWINDOW_H
#define APPBASEWINDOW_H

#include <QWidget>
#include "global.h"
#include "projectmanagewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AppBaseWindow;
}
QT_END_NAMESPACE

class AppBaseWindow : public QWidget
{
    Q_OBJECT

public:
    AppBaseWindow(QWidget *parent = nullptr);
    ~AppBaseWindow();
    void setUserName(const QString& username);


private:
    /**
     * @brief initComboBox
     * 初始化 ComboBox 使其样式生效
     */
    void initComboBox();

    /**
     * @brief initTableView
     * 初始化 tableView
     */
    void initTableView();

    /**
     * @brief initTabWidget
     * 初始化成员变量tabwidget
    */
    void initTabWidget();

    /**
     * @brief initReviseTable
     * 初始化修改表格
     */
    void initReviseTable();

    /**
     * @brief AppBaseWindow::parseNumRange
     * 分割 num-num 为最大最小值
     * @param priceRange
     * @return 返回pair
     */
    std::pair<int, int> parseNumRange(const QString& priceRange);

private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_rbtn_exactSearch_clicked();

    void on_rbtn_fuzzySearch_clicked();

    void on_btn_projectNew_clicked();

    void on_rbt_recent_clicked();

    void on_rbt_favour_clicked();

    void slot_handle_create_project();

    void slot_handle_insert_blogger();

    void on_btn_upload_clicked();

    void on_table_infoQuery_doubleClicked(const QModelIndex &index);

    void on_table_recent_doubleClicked(const QModelIndex &index);

    void slot_handle_tabWidget_close();


    void on_btn_reviewPass_clicked();

    void on_edt_Projectsearch_textChanged(const QString &arg1);

    void on_table_projectSearch_doubleClicked(const QModelIndex &index);

    void on_btn_search_clicked();

    void on_btn_clearSearch_clicked();

signals:
    void sig_insert_blogger();


private:
    Ui::AppBaseWindow *ui;

    /**
     * @brief _bloggers_model
     * 博主数据模型
     */
    MySqlQueryModel* _bloggers_model;
    /**
     * @brief _projects_model
     * 项目数据模型
     */
    MySqlQueryModel* _projects_model;

    MySqlQueryModel* _projects_serch_model;

    QStandardItemModel* _revise_model;

    MyTabWidget* _Tab;

    QMap<QString, ProjectManageWidget*> _prjManageTabMap;
    
    QString _current_user;

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // APPBASEWINDOW_H
