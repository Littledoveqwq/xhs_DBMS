#include "configmgr.h"

ConfigMgr::ConfigMgr() {
    //获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    //拼接文件名
    QString fileName = "config.ini";
    //toNativeSeparators调用这个转化成系统原生的斜杠, 防止程序无法识别路径
    _config_path = QDir::toNativeSeparators(app_path + QDir::separator() + fileName);
    _settings = new QSettings(_config_path, QSettings::IniFormat);
}

ConfigMgr::~ConfigMgr()
{

}

MySQLInfo ConfigMgr::getMysqlInfo()
{
    MySQLInfo sqlInfo;
    sqlInfo.host = _settings->value("MySQL/host").toString();
    sqlInfo.port = _settings->value("MySQL/port").toInt();
    sqlInfo.database_name = _settings->value("MySQL/database_name").toString();
    sqlInfo.user_name = _settings->value("MySQL/user_name").toString();
    sqlInfo.passwd = _settings->value("MySQL/passwd").toString();
    return sqlInfo;
}
