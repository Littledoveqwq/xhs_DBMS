#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    // QStringList drivers = QSqlDatabase::drivers();
    // for(QString d : drivers){
    //     qDebug() << d;
    // }
    return a.exec();
}
