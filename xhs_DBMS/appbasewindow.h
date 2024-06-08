#ifndef APPBASEWINDOW_H
#define APPBASEWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>

class CustomSqlQueryModel : public QSqlQueryModel
{
public:
    CustomSqlQueryModel(QObject *parent = nullptr)
        : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        } else {
            return QSqlQueryModel::data(index, role);
        }
    }

    void refresh() {
        setQuery(query().executedQuery());
    }
};

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

signals:
    void sig_insert_blogger();


private:
    Ui::AppBaseWindow *ui;

    /**
     * @brief _bloggers_model
     * 博主数据模型
     */
    CustomSqlQueryModel* _bloggers_model;
    /**
     * @brief _projects_model
     * 项目数据模型
     */
    CustomSqlQueryModel* _projects_model;
};
#endif // APPBASEWINDOW_H
