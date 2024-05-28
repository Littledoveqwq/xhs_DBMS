#ifndef INPUTBASEDIALOG_H
#define INPUTBASEDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QObject>
#include <QMessageBox>

class InputBaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputBaseDialog(QWidget *parent = nullptr, QWidget *topWidget = nullptr);
    virtual ~InputBaseDialog();
    void setTopWidget(QWidget *topWidget);


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QPoint last;
    int canmove;
    QWidget *widgetTop;
};


#endif // INPUTBASEDIALOG_H
