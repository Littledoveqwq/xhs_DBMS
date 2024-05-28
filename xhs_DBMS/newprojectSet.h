#ifndef NEWPROJECTSET_H
#define NEWPROJECTSET_H

#include <QWidget>

namespace Ui {
class NewProject;
}

class NewProject : public QWidget
{
    Q_OBJECT

public:
    explicit NewProject(QWidget *parent = nullptr);
    ~NewProject();

private:
    Ui::NewProject *ui;
};

#endif // NEWPROJECTSET_H
