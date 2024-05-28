#include "SetnewProject.h"
#include "ui_SetnewProject.h"

SetnewProject::SetnewProject(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetnewProject)
{
    ui->setupUi(this);
}

SetnewProject::~SetnewProject()
{
    delete ui;
}
