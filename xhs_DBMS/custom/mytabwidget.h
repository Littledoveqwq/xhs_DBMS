#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    MyTabWidget(QWidget* parent = nullptr);

    void removeAllTabs();
signals:
    void sig_tabWidget_close();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // MYTABWIDGET_H
