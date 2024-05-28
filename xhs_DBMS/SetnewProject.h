#ifndef SETNEWPROJECT_H
#define SETNEWPROJECT_H

#include <QDialog>

namespace Ui {
class SetnewProject;
}

class SetnewProject : public QDialog
{
    Q_OBJECT

public:
    explicit SetnewProject(QWidget *parent = nullptr);
    ~SetnewProject();

private:
    Ui::SetnewProject *ui;
};

#endif // SETNEWPROJECT_H
