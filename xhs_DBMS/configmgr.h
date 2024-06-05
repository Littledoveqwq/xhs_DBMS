#ifndef CONFIGMGR_H
#define CONFIGMGR_H
#include "global.h"
#include "singleton.h"
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QDir>

class ConfigMgr : public Singleton<ConfigMgr>
{
public:
    ConfigMgr();
    ~ConfigMgr();
    MySQLInfo getMysqlInfo();

private:
    QString _config_path;
    QSettings* _settings;
};

#endif // CONFIGMGR_H
